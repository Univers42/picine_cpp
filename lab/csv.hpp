/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   csv.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 01:58:35 by marvin            #+#    #+#             */
/*   Updated: 2025/12/24 01:58:35 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CSV_HPP
#define CSV_HPP

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <numeric>
#include <cmath>

namespace CSV {

// ============================================================================
// TYPE DETECTION AND INFERENCE
// ============================================================================

enum class DataType {
    STRING,
    INTEGER,
    DOUBLE,
    BOOLEAN,
    DATE,
    EMPTY
};

class TypeInference {
public:
    static DataType inferType(const std::string& value) {
        if (value.empty()) return DataType::EMPTY;
        
        // Boolean
        std::string lower = toLower(value);
        if (lower == "true" || lower == "false" || 
            lower == "yes" || lower == "no" || 
            lower == "1" || lower == "0") {
            return DataType::BOOLEAN;
        }
        
        // Integer
        if (isInteger(value)) return DataType::INTEGER;
        
        // Double
        if (isDouble(value)) return DataType::DOUBLE;
        
        // Date (basic patterns)
        if (isDate(value)) return DataType::DATE;
        
        return DataType::STRING;
    }
    
    static bool isInteger(const std::string& s) {
        if (s.empty()) return false;
        size_t start = (s[0] == '-' || s[0] == '+') ? 1 : 0;
        if (start >= s.length()) return false;
        
        for (size_t i = start; i < s.length(); ++i) {
            if (!std::isdigit(s[i])) return false;
        }
        return true;
    }
    
    static bool isDouble(const std::string& s) {
        try {
            size_t pos;
            std::stod(s, &pos);
            return pos == s.length();
        } catch (...) {
            return false;
        }
    }
    
    static bool isDate(const std::string& s) {
        // Basic date format detection: YYYY-MM-DD, DD/MM/YYYY, etc.
        return (s.find('-') != std::string::npos || s.find('/') != std::string::npos) 
               && s.length() >= 8 && s.length() <= 10;
    }

private:
    static std::string toLower(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
};

// ============================================================================
// CSV ROW
// ============================================================================

class Row {
public:
    Row() {}
    explicit Row(const std::vector<std::string>& values) : _values(values) {}
    
    size_t size() const { return _values.size(); }
    bool empty() const { return _values.empty(); }
    
    const std::string& operator[](size_t index) const {
        static const std::string empty;
        return (index < _values.size()) ? _values[index] : empty;
    }
    
    std::string& operator[](size_t index) {
        if (index >= _values.size()) {
            _values.resize(index + 1);
        }
        return _values[index];
    }
    
    const std::string& get(const std::string& columnName) const {
        auto it = _columnMap.find(columnName);
        static const std::string empty;
        return (it != _columnMap.end()) ? _values[it->second] : empty;
    }
    
    void set(const std::string& columnName, const std::string& value) {
        auto it = _columnMap.find(columnName);
        if (it != _columnMap.end()) {
            _values[it->second] = value;
        }
    }
    
    void setColumnMap(const std::map<std::string, size_t>& columnMap) {
        _columnMap = columnMap;
    }
    
    const std::vector<std::string>& values() const { return _values; }
    
    // Type conversion helpers
    int getInt(size_t index, int defaultValue = 0) const {
        try {
            return (index < _values.size()) ? std::stoi(_values[index]) : defaultValue;
        } catch (...) {
            return defaultValue;
        }
    }
    
    double getDouble(size_t index, double defaultValue = 0.0) const {
        try {
            return (index < _values.size()) ? std::stod(_values[index]) : defaultValue;
        } catch (...) {
            return defaultValue;
        }
    }
    
    bool getBool(size_t index, bool defaultValue = false) const {
        if (index >= _values.size()) return defaultValue;
        std::string lower = _values[index];
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return (lower == "true" || lower == "yes" || lower == "1");
    }

private:
    std::vector<std::string> _values;
    std::map<std::string, size_t> _columnMap;
};

// ============================================================================
// CSV PARSER - ROBUST PARSING ENGINE
// ============================================================================

class Parser {
public:
    struct Options {
        char delimiter = ',';
        char quote = '"';
        char escape = '\\';
        bool hasHeader = true;
        bool trimWhitespace = true;
        bool skipEmptyLines = true;
        bool strictQuotes = false;
        size_t skipLines = 0;
        
        static Options RFC4180() {
            Options opt;
            opt.delimiter = ',';
            opt.quote = '"';
            opt.escape = '"';  // RFC 4180 uses double quote for escaping
            opt.hasHeader = true;
            return opt;
        }
        
        static Options TSV() {
            Options opt;
            opt.delimiter = '\t';
            return opt;
        }
        
        static Options Excel() {
            Options opt;
            opt.delimiter = ',';
            opt.quote = '"';
            opt.escape = '"';
            opt.hasHeader = true;
            return opt;
        }
    };
    
    Options options;
    
    Parser(const Options& opt = Options()) : options(opt) {}
    
    std::vector<std::string> parseLine(const std::string& line) const {
        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;
        bool escapeNext = false;
        
        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            
            if (escapeNext) {
                field += c;
                escapeNext = false;
                continue;
            }
            
            if (c == options.escape && i + 1 < line.length()) {
                if (line[i + 1] == options.quote) {
                    escapeNext = true;
                    continue;
                }
            }
            
            if (c == options.quote) {
                if (options.strictQuotes) {
                    inQuotes = !inQuotes;
                } else {
                    // Handle quotes more flexibly
                    if (inQuotes && i + 1 < line.length() && line[i + 1] == options.quote) {
                        field += options.quote;
                        ++i;
                    } else {
                        inQuotes = !inQuotes;
                    }
                }
                continue;
            }
            
            if (c == options.delimiter && !inQuotes) {
                if (options.trimWhitespace) {
                    field = trim(field);
                }
                fields.push_back(field);
                field.clear();
                continue;
            }
            
            field += c;
        }
        
        // Add last field
        if (options.trimWhitespace) {
            field = trim(field);
        }
        fields.push_back(field);
        
        return fields;
    }
    
    static std::string trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }
};

// ============================================================================
// CSV DOCUMENT - MAIN DATA STRUCTURE
// ============================================================================

class Document {
public:
    Document() {}
    
    // Load from file
    bool load(const std::string& filename, const Parser::Options& options = Parser::Options()) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            _error = "Cannot open file: " + filename;
            return false;
        }
        
        return loadFromStream(file, options);
    }
    
    // Load from stream
    bool loadFromStream(std::istream& stream, const Parser::Options& options = Parser::Options()) {
        _rows.clear();
        _headers.clear();
        _columnMap.clear();
        _error.clear();
        
        Parser parser(options);
        std::string line;
        size_t lineNum = 0;
        
        // Skip initial lines
        for (size_t i = 0; i < options.skipLines; ++i) {
            if (!std::getline(stream, line)) return true;
        }
        
        // Read header
        if (options.hasHeader) {
            if (!std::getline(stream, line)) {
                _error = "Empty file or missing header";
                return false;
            }
            _headers = parser.parseLine(line);
            
            // Build column map
            for (size_t i = 0; i < _headers.size(); ++i) {
                _columnMap[_headers[i]] = i;
            }
        }
        
        // Read data rows
        while (std::getline(stream, line)) {
            ++lineNum;
            
            if (options.skipEmptyLines && line.empty()) {
                continue;
            }
            
            try {
                std::vector<std::string> fields = parser.parseLine(line);
                
                if (!options.hasHeader && _headers.empty()) {
                    // Generate default headers
                    for (size_t i = 0; i < fields.size(); ++i) {
                        _headers.push_back("Column" + std::to_string(i));
                        _columnMap[_headers[i]] = i;
                    }
                }
                
                Row row(fields);
                row.setColumnMap(_columnMap);
                _rows.push_back(row);
                
            } catch (const std::exception& e) {
                _error = "Parse error at line " + std::to_string(lineNum) + ": " + e.what();
                return false;
            }
        }
        
        return true;
    }
    
    // Save to file
    bool save(const std::string& filename, const Parser::Options& options = Parser::Options()) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        return saveToStream(file, options);
    }
    
    // Save to stream
    bool saveToStream(std::ostream& stream, const Parser::Options& options = Parser::Options()) const {
        // Write header
        if (options.hasHeader && !_headers.empty()) {
            for (size_t i = 0; i < _headers.size(); ++i) {
                stream << escapeField(_headers[i], options);
                if (i < _headers.size() - 1) stream << options.delimiter;
            }
            stream << "\n";
        }
        
        // Write rows
        for (const auto& row : _rows) {
            for (size_t i = 0; i < row.size(); ++i) {
                stream << escapeField(row[i], options);
                if (i < row.size() - 1) stream << options.delimiter;
            }
            stream << "\n";
        }
        
        return true;
    }
    
    // Accessors
    const std::vector<Row>& rows() const { return _rows; }
    std::vector<Row>& rows() { return _rows; }
    const std::vector<std::string>& headers() const { return _headers; }
    size_t rowCount() const { return _rows.size(); }
    size_t columnCount() const { return _headers.size(); }
    const std::string& error() const { return _error; }
    
    // Get column index
    int getColumnIndex(const std::string& name) const {
        auto it = _columnMap.find(name);
        return (it != _columnMap.end()) ? (int)it->second : -1;
    }
    
    // Add row
    void addRow(const Row& row) {
        _rows.push_back(row);
    }
    
    // Add column (header)
    void addColumn(const std::string& name) {
        _headers.push_back(name);
        _columnMap[name] = _headers.size() - 1;
    }
    
    // Set headers
    void setHeaders(const std::vector<std::string>& headers) {
        _headers = headers;
        _columnMap.clear();
        for (size_t i = 0; i < headers.size(); ++i) {
            _columnMap[headers[i]] = i;
        }
    }
    
    // Clear all data
    void clear() {
        _rows.clear();
        _headers.clear();
        _columnMap.clear();
        _error.clear();
    }

private:
    std::vector<Row> _rows;
    std::vector<std::string> _headers;
    std::map<std::string, size_t> _columnMap;
    mutable std::string _error;
    
    std::string escapeField(const std::string& field, const Parser::Options& options) const {
        bool needsQuotes = (field.find(options.delimiter) != std::string::npos ||
                           field.find(options.quote) != std::string::npos ||
                           field.find('\n') != std::string::npos);
        
        if (!needsQuotes) return field;
        
        std::string escaped;
        escaped += options.quote;
        for (char c : field) {
            if (c == options.quote) {
                escaped += options.quote;
                escaped += options.quote;
            } else {
                escaped += c;
            }
        }
        escaped += options.quote;
        return escaped;
    }
};

// ============================================================================
// FILTERING AND TRANSFORMATION
// ============================================================================

class Filter {
public:
    using Predicate = std::function<bool(const Row&)>;
    
    // Filter rows by predicate
    static Document filter(const Document& doc, Predicate pred) {
        Document result;
        result.setHeaders(doc.headers());
        
        for (const auto& row : doc.rows()) {
            if (pred(row)) {
                result.addRow(row);
            }
        }
        
        return result;
    }
    
    // Filter by column value
    static Document where(const Document& doc, const std::string& column, const std::string& value) {
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0) return Document();
        
        return filter(doc, [colIndex, value](const Row& row) {
            return row[colIndex] == value;
        });
    }
    
    // Select specific columns
    static Document select(const Document& doc, const std::vector<std::string>& columns) {
        Document result;
        result.setHeaders(columns);
        
        std::vector<int> indices;
        for (const auto& col : columns) {
            indices.push_back(doc.getColumnIndex(col));
        }
        
        for (const auto& row : doc.rows()) {
            std::vector<std::string> newValues;
            for (int idx : indices) {
                newValues.push_back(idx >= 0 ? row[idx] : "");
            }
            Row newRow(newValues);
            result.addRow(newRow);
        }
        
        return result;
    }
    
    // Sort by column
    static Document sortBy(const Document& doc, const std::string& column, bool ascending = true) {
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0) return doc;
        
        Document result;
        result.setHeaders(doc.headers());
        
        std::vector<Row> sortedRows = doc.rows();
        std::sort(sortedRows.begin(), sortedRows.end(), [colIndex, ascending](const Row& a, const Row& b) {
            if (ascending) return a[colIndex] < b[colIndex];
            return a[colIndex] > b[colIndex];
        });
        
        for (const auto& row : sortedRows) {
            result.addRow(row);
        }
        
        return result;
    }
    
    // Limit number of rows
    static Document limit(const Document& doc, size_t maxRows) {
        Document result;
        result.setHeaders(doc.headers());
        
        size_t count = 0;
        for (const auto& row : doc.rows()) {
            if (count++ >= maxRows) break;
            result.addRow(row);
        }
        
        return result;
    }
    
    // Skip rows
    static Document skip(const Document& doc, size_t skipCount) {
        Document result;
        result.setHeaders(doc.headers());
        
        size_t count = 0;
        for (const auto& row : doc.rows()) {
            if (count++ < skipCount) continue;
            result.addRow(row);
        }
        
        return result;
    }
    
    // Group by column
    static std::map<std::string, Document> groupBy(const Document& doc, const std::string& column) {
        std::map<std::string, Document> groups;
        int colIndex = doc.getColumnIndex(column);
        
        if (colIndex < 0) return groups;
        
        for (const auto& row : doc.rows()) {
            std::string key = row[colIndex];
            
            if (groups.find(key) == groups.end()) {
                groups[key] = Document();
                groups[key].setHeaders(doc.headers());
            }
            
            groups[key].addRow(row);
        }
        
        return groups;
    }
};

// ============================================================================
// AGGREGATION FUNCTIONS
// ============================================================================

class Aggregate {
public:
    // Count rows
    static size_t count(const Document& doc) {
        return doc.rowCount();
    }
    
    // Count non-empty values in column
    static size_t countValues(const Document& doc, const std::string& column) {
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0) return 0;
        
        size_t count = 0;
        for (const auto& row : doc.rows()) {
            if (!row[colIndex].empty()) ++count;
        }
        return count;
    }
    
    // Sum numeric column
    static double sum(const Document& doc, const std::string& column) {
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0) return 0.0;
        
        double total = 0.0;
        for (const auto& row : doc.rows()) {
            total += row.getDouble(colIndex);
        }
        return total;
    }
    
    // Average of numeric column
    static double average(const Document& doc, const std::string& column) {
        size_t count = doc.rowCount();
        return count > 0 ? sum(doc, column) / count : 0.0;
    }
    
    // Minimum value
    static double min(const Document& doc, const std::string& column) {
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0 || doc.rowCount() == 0) return 0.0;
        
        double minVal = std::numeric_limits<double>::max();
        for (const auto& row : doc.rows()) {
            double val = row.getDouble(colIndex);
            minVal = std::min(minVal, val);
        }
        return minVal;
    }
    
    // Maximum value
    static double max(const Document& doc, const std::string& column) {
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0 || doc.rowCount() == 0) return 0.0;
        
        double maxVal = std::numeric_limits<double>::lowest();
        for (const auto& row : doc.rows()) {
            double val = row.getDouble(colIndex);
            maxVal = std::max(maxVal, val);
        }
        return maxVal;
    }
    
    // Standard deviation
    static double stddev(const Document& doc, const std::string& column) {
        double avg = average(doc, column);
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0 || doc.rowCount() == 0) return 0.0;
        
        double variance = 0.0;
        for (const auto& row : doc.rows()) {
            double diff = row.getDouble(colIndex) - avg;
            variance += diff * diff;
        }
        
        return std::sqrt(variance / doc.rowCount());
    }
    
    // Distinct values in column
    static std::vector<std::string> distinct(const Document& doc, const std::string& column) {
        int colIndex = doc.getColumnIndex(column);
        if (colIndex < 0) return {};
        
        std::vector<std::string> unique;
        for (const auto& row : doc.rows()) {
            std::string val = row[colIndex];
            if (std::find(unique.begin(), unique.end(), val) == unique.end()) {
                unique.push_back(val);
            }
        }
        
        return unique;
    }
};

// ============================================================================
// DATA VALIDATION
// ============================================================================

class Validator {
public:
    struct ValidationResult {
        bool valid = true;
        std::vector<std::string> errors;
        std::vector<size_t> errorRows;
    };
    
    using Rule = std::function<bool(const std::string&)>;
    
    // Validate column with custom rule
    static ValidationResult validate(const Document& doc, const std::string& column, Rule rule) {
        ValidationResult result;
        int colIndex = doc.getColumnIndex(column);
        
        if (colIndex < 0) {
            result.valid = false;
            result.errors.push_back("Column not found: " + column);
            return result;
        }
        
        for (size_t i = 0; i < doc.rowCount(); ++i) {
            if (!rule(doc.rows()[i][colIndex])) {
                result.valid = false;
                result.errors.push_back("Validation failed at row " + std::to_string(i));
                result.errorRows.push_back(i);
            }
        }
        
        return result;
    }
    
    // Check for required fields (non-empty)
    static ValidationResult checkRequired(const Document& doc, const std::vector<std::string>& columns) {
        ValidationResult result;
        
        for (const auto& column : columns) {
            auto colResult = validate(doc, column, [](const std::string& val) {
                return !val.empty();
            });
            
            if (!colResult.valid) {
                result.valid = false;
                result.errors.insert(result.errors.end(), 
                    colResult.errors.begin(), colResult.errors.end());
            }
        }
        
        return result;
    }
    
    // Check column type consistency
    static ValidationResult checkType(const Document& doc, const std::string& column, DataType expectedType) {
        return validate(doc, column, [expectedType](const std::string& val) {
            return val.empty() || TypeInference::inferType(val) == expectedType;
        });
    }
};

} // namespace CSV

#endif // CSV_HPP