/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:13:46 by marvin            #+#    #+#             */
/*   Updated: 2025/12/24 02:13:46 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_HPP
# define DATABASE_HPP

#ifndef DATABASE_SYSTEM_HPP
#define DATABASE_SYSTEM_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cstdint>

// ============================================================================
// UNICODE UTILITIES
// ============================================================================

namespace Unicode {
    // Box drawing characters
    struct BoxChars {
        std::string topLeft = "┌";
        std::string topRight = "┐";
        std::string bottomLeft = "└";
        std::string bottomRight = "┘";
        std::string horizontal = "─";
        std::string vertical = "│";
        std::string cross = "┼";
        std::string teeLeft = "├";
        std::string teeRight = "┤";
        std::string teeTop = "┬";
        std::string teeBottom = "┴";
        
        // Heavy variants
        static BoxChars heavy() {
            BoxChars b;
            b.topLeft = "┏"; b.topRight = "┓";
            b.bottomLeft = "┗"; b.bottomRight = "┛";
            b.horizontal = "━"; b.vertical = "┃";
            b.cross = "╋"; b.teeLeft = "┣";
            b.teeRight = "┫"; b.teeTop = "┳";
            b.teeBottom = "┻";
            return b;
        }
        
        // Double line variants
        static BoxChars doubleLine() {
            BoxChars b;
            b.topLeft = "╔"; b.topRight = "╗";
            b.bottomLeft = "╚"; b.bottomRight = "╝";
            b.horizontal = "═"; b.vertical = "║";
            b.cross = "╬"; b.teeLeft = "╠";
            b.teeRight = "╣"; b.teeTop = "╦";
            b.teeBottom = "╩";
            return b;
        }
        
        // Rounded corners
        static BoxChars rounded() {
            BoxChars b;
            b.topLeft = "╭"; b.topRight = "╮";
            b.bottomLeft = "╰"; b.bottomRight = "╯";
            return b;
        }
    };
    
    // Calculate display width of UTF-8 string
    size_t displayWidth(const std::string& str) {
        size_t width = 0;
        for (size_t i = 0; i < str.length(); ) {
            unsigned char c = str[i];
            if ((c & 0x80) == 0) { i += 1; width += 1; }
            else if ((c & 0xE0) == 0xC0) { i += 2; width += 1; }
            else if ((c & 0xF0) == 0xE0) { i += 3; width += 1; }
            else if ((c & 0xF8) == 0xF0) { i += 4; width += 2; }
            else { i += 1; }
        }
        return width;
    }
    
    // Pad string to specific display width
    std::string pad(const std::string& str, size_t width, char align = 'l') {
        size_t currentWidth = displayWidth(str);
        if (currentWidth >= width) return str;
        
        size_t padding = width - currentWidth;
        if (align == 'c') {
            size_t left = padding / 2;
            size_t right = padding - left;
            return std::string(left, ' ') + str + std::string(right, ' ');
        } else if (align == 'r') {
            return std::string(padding, ' ') + str;
        } else {
            return str + std::string(padding, ' ');
        }
    }
}

// ============================================================================
// COLOR AND STYLE UTILITIES
// ============================================================================

namespace Style {
    // ANSI color codes
    struct Color {
        int code;
        
        static Color Black() { return {30}; }
        static Color Red() { return {31}; }
        static Color Green() { return {32}; }
        static Color Yellow() { return {33}; }
        static Color Blue() { return {34}; }
        static Color Magenta() { return {35}; }
        static Color Cyan() { return {36}; }
        static Color White() { return {37}; }
        static Color BrightBlack() { return {90}; }
        static Color BrightRed() { return {91}; }
        static Color BrightGreen() { return {92}; }
        static Color BrightYellow() { return {93}; }
        static Color BrightBlue() { return {94}; }
        static Color BrightMagenta() { return {95}; }
        static Color BrightCyan() { return {96}; }
        static Color BrightWhite() { return {97}; }
        static Color Default() { return {39}; }
        
        static Color RGB(int r, int g, int b) {
            // Returns approximate ANSI color
            return {30 + (r > 127 ? 0 : 0) + (g > 127 ? 2 : 0) + (b > 127 ? 4 : 0)};
        }
    };
    
    struct CellStyle {
        Color foreground = Color::Default();
        Color background = Color{49}; // Default background
        bool bold = false;
        bool italic = false;
        bool underline = false;
        
        std::string apply(const std::string& text) const {
            std::ostringstream oss;
            oss << "\033[";
            bool first = true;
            
            if (bold) { oss << (first ? "" : ";") << "1"; first = false; }
            if (italic) { oss << (first ? "" : ";") << "3"; first = false; }
            if (underline) { oss << (first ? "" : ";") << "4"; first = false; }
            if (foreground.code != 39) { oss << (first ? "" : ";") << foreground.code; first = false; }
            if (background.code != 49) { oss << (first ? "" : ";") << background.code; first = false; }
            
            oss << "m" << text << "\033[0m";
            return oss.str();
        }
    };
}

// ============================================================================
// COLUMN TYPE SYSTEM
// ============================================================================

enum class ColumnType {
    STRING,
    INTEGER,
    DOUBLE,
    DATE,
    BOOLEAN
};

enum class Alignment {
    LEFT,
    CENTER,
    RIGHT
};

class Column {
public:
    Column(const std::string& name, ColumnType type = ColumnType::STRING, 
           Alignment align = Alignment::LEFT)
        : _name(name), _type(type), _alignment(align), _width(0) {}
    
    const std::string& name() const { return _name; }
    ColumnType type() const { return _type; }
    Alignment alignment() const { return _alignment; }
    size_t width() const { return _width; }
    void setWidth(size_t w) { _width = w; }
    
    char getAlignChar() const {
        switch (_alignment) {
            case Alignment::CENTER: return 'c';
            case Alignment::RIGHT: return 'r';
            default: return 'l';
        }
    }
    
    std::string format(const std::string& value) const {
        switch (_type) {
            case ColumnType::INTEGER:
            case ColumnType::DOUBLE:
                return value;
            case ColumnType::BOOLEAN:
                return (value == "1" || value == "true") ? "✓" : "✗";
            default:
                return value;
        }
    }

private:
    std::string _name;
    ColumnType _type;
    Alignment _alignment;
    size_t _width;
};

// ============================================================================
// ROW - DATA RECORD
// ============================================================================

class Row {
public:
    Row() {}
    
    void setValue(const std::string& columnName, const std::string& value) {
        _data[columnName] = value;
    }
    
    std::string getValue(const std::string& columnName) const {
        auto it = _data.find(columnName);
        return (it != _data.end()) ? it->second : "";
    }
    
    const std::map<std::string, std::string>& data() const { return _data; }

private:
    std::map<std::string, std::string> _data;
};

// ============================================================================
// TABLE - CONTAINER FOR COLUMNS AND ROWS
// ============================================================================

class Table {
public:
    Table() {}
    
    void addColumn(const Column& col) {
        _columns.push_back(col);
    }
    
    void addRow(const Row& row) {
        _rows.push_back(row);
    }
    
    const std::vector<Column>& columns() const { return _columns; }
    std::vector<Column>& columns() { return _columns; }
    const std::vector<Row>& rows() const { return _rows; }
    
    void clear() {
        _rows.clear();
    }
    
    size_t columnCount() const { return _columns.size(); }
    size_t rowCount() const { return _rows.size(); }
    
    // Auto-calculate column widths based on content
    void calculateColumnWidths() {
        for (auto& col : _columns) {
            size_t maxWidth = Unicode::displayWidth(col.name());
            
            for (const auto& row : _rows) {
                std::string value = col.format(row.getValue(col.name()));
                size_t width = Unicode::displayWidth(value);
                maxWidth = std::max(maxWidth, width);
            }
            
            col.setWidth(maxWidth);
        }
    }

private:
    std::vector<Column> _columns;
    std::vector<Row> _rows;
};

// ============================================================================
// RENDER CONFIGURATION
// ============================================================================

struct RenderConfig {
    Unicode::BoxChars boxChars;
    Style::CellStyle headerStyle;
    Style::CellStyle cellStyle;
    Style::CellStyle footerStyle;
    bool showHeader = true;
    bool showFooter = false;
    std::string footerText = "";
    int padding = 1;
    bool autoWidth = true;
    
    RenderConfig() {
        // Default: bold cyan header
        headerStyle.bold = true;
        headerStyle.foreground = Style::Color::BrightCyan();
    }
    
    static RenderConfig elegant() {
        RenderConfig cfg;
        cfg.boxChars = Unicode::BoxChars::doubleLine();
        cfg.headerStyle.bold = true;
        cfg.headerStyle.foreground = Style::Color::BrightWhite();
        cfg.headerStyle.background = Style::Color{44}; // Cyan bg
        return cfg;
    }
    
    static RenderConfig minimal() {
        RenderConfig cfg;
        cfg.padding = 0;
        cfg.headerStyle.bold = true;
        return cfg;
    }
};

// ============================================================================
// TABLE RENDERER - BUFFERED OUTPUT
// ============================================================================

class TableRenderer {
public:
    TableRenderer(const RenderConfig& config = RenderConfig())
        : _config(config) {}
    
    std::string render(Table& table) {
        if (_config.autoWidth) {
            table.calculateColumnWidths();
        }
        
        _buffer.clear();
        _buffer.str("");
        
        renderTopBorder(table);
        if (_config.showHeader) {
            renderHeader(table);
            renderHeaderSeparator(table);
        }
        renderRows(table);
        if (_config.showFooter) {
            renderFooterSeparator(table);
            renderFooter(table);
        }
        renderBottomBorder(table);
        
        return _buffer.str();
    }
    
    void setConfig(const RenderConfig& config) {
        _config = config;
    }

private:
    RenderConfig _config;
    std::ostringstream _buffer;
    
    void renderTopBorder(const Table& table) {
        _buffer << _config.boxChars.topLeft;
        
        const auto& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            size_t width = cols[i].width() + 2 * _config.padding;
            _buffer << repeat(_config.boxChars.horizontal, width);
            
            if (i < cols.size() - 1) {
                _buffer << _config.boxChars.teeTop;
            }
        }
        
        _buffer << _config.boxChars.topRight << "\n";
    }
    
    void renderHeader(const Table& table) {
        _buffer << _config.boxChars.vertical;
        
        const auto& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            std::string content = Unicode::pad(
                cols[i].name(),
                cols[i].width(),
                'c'
            );
            
            _buffer << repeat(" ", _config.padding);
            _buffer << _config.headerStyle.apply(content);
            _buffer << repeat(" ", _config.padding);
            _buffer << _config.boxChars.vertical;
        }
        
        _buffer << "\n";
    }
    
    void renderHeaderSeparator(const Table& table) {
        _buffer << _config.boxChars.teeLeft;
        
        const auto& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            size_t width = cols[i].width() + 2 * _config.padding;
            _buffer << repeat(_config.boxChars.horizontal, width);
            
            if (i < cols.size() - 1) {
                _buffer << _config.boxChars.cross;
            }
        }
        
        _buffer << _config.boxChars.teeRight << "\n";
    }
    
    void renderRows(const Table& table) {
        const auto& cols = table.columns();
        const auto& rows = table.rows();
        
        for (size_t r = 0; r < rows.size(); ++r) {
            _buffer << _config.boxChars.vertical;
            
            for (size_t c = 0; c < cols.size(); ++c) {
                std::string value = cols[c].format(rows[r].getValue(cols[c].name()));
                std::string content = Unicode::pad(
                    value,
                    cols[c].width(),
                    cols[c].getAlignChar()
                );
                
                _buffer << repeat(" ", _config.padding);
                _buffer << _config.cellStyle.apply(content);
                _buffer << repeat(" ", _config.padding);
                _buffer << _config.boxChars.vertical;
            }
            
            _buffer << "\n";
        }
    }
    
    void renderFooterSeparator(const Table& table) {
        _buffer << _config.boxChars.teeLeft;
        
        const auto& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            size_t width = cols[i].width() + 2 * _config.padding;
            _buffer << repeat(_config.boxChars.horizontal, width);
            
            if (i < cols.size() - 1) {
                _buffer << _config.boxChars.cross;
            }
        }
        
        _buffer << _config.boxChars.teeRight << "\n";
    }
    
    void renderFooter(const Table& table) {
        _buffer << _config.boxChars.vertical;
        
        size_t totalWidth = 0;
        const auto& cols = table.columns();
        for (const auto& col : cols) {
            totalWidth += col.width() + 2 * _config.padding + 1;
        }
        totalWidth -= 1; // Remove last separator
        
        std::string content = Unicode::pad(_config.footerText, totalWidth - 2, 'c');
        _buffer << " " << _config.footerStyle.apply(content) << " ";
        _buffer << _config.boxChars.vertical << "\n";
    }
    
    void renderBottomBorder(const Table& table) {
        _buffer << _config.boxChars.bottomLeft;
        
        const auto& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            size_t width = cols[i].width() + 2 * _config.padding;
            _buffer << repeat(_config.boxChars.horizontal, width);
            
            if (i < cols.size() - 1) {
                _buffer << _config.boxChars.teeBottom;
            }
        }
        
        _buffer << _config.boxChars.bottomRight << "\n";
    }
    
    std::string repeat(const std::string& s, size_t n) {
        std::string result;
        for (size_t i = 0; i < n; ++i) {
            result += s;
        }
        return result;
    }
};

// ============================================================================
// CSV PARSER
// ============================================================================

class CsvParser {
public:
    static Table parse(const std::string& path, bool hasHeader = true) {
        Table table;
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + path);
        }
        
        std::string line;
        bool isFirstLine = true;
        std::vector<std::string> headers;
        
        while (std::getline(file, line)) {
            auto fields = parseLine(line);
            
            if (isFirstLine && hasHeader) {
                headers = fields;
                for (const auto& header : headers) {
                    table.addColumn(Column(header));
                }
                isFirstLine = false;
            } else {
                if (headers.empty()) {
                    // Generate default headers
                    for (size_t i = 0; i < fields.size(); ++i) {
                        headers.push_back("Column" + std::to_string(i + 1));
                        table.addColumn(Column(headers.back()));
                    }
                }
                
                Row row;
                for (size_t i = 0; i < fields.size() && i < headers.size(); ++i) {
                    row.setValue(headers[i], fields[i]);
                }
                table.addRow(row);
            }
        }
        
        return table;
    }

private:
    static std::vector<std::string> parseLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;
        
        for (size_t i = 0; i < line.length(); ++i) {
            char c = line[i];
            
            if (c == '"') {
                if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                fields.push_back(trim(field));
                field.clear();
            } else {
                field += c;
            }
        }
        
        fields.push_back(trim(field));
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
// DATABASE - HIGH LEVEL ORCHESTRATOR
// ============================================================================

class Database {
public:
    Database() {}
    
    void loadFromCsv(const std::string& path, bool hasHeader = true) {
        _table = CsvParser::parse(path, hasHeader);
    }
    
    void addColumn(const std::string& name, ColumnType type = ColumnType::STRING,
                   Alignment align = Alignment::LEFT) {
        _table.addColumn(Column(name, type, align));
    }
    
    void addRow(const std::map<std::string, std::string>& data) {
        Row row;
        for (const auto& pair : data) {
            row.setValue(pair.first, pair.second);
        }
        _table.addRow(row);
    }
    
    std::string render(const RenderConfig& config = RenderConfig()) {
        TableRenderer renderer(config);
        return renderer.render(_table);
    }
    
    Table& table() { return _table; }
    const Table& table() const { return _table; }
    
    // Query methods
    std::vector<Row> where(const std::string& column, const std::string& value) const {
        std::vector<Row> results;
        for (const auto& row : _table.rows()) {
            if (row.getValue(column) == value) {
                results.push_back(row);
            }
        }
        return results;
    }
    
    size_t count() const { return _table.rowCount(); }

private:
    Table _table;
};

#endif // DATABASE_SYSTEM_HPP