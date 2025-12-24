/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Database_utils.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:34:49 by marvin            #+#    #+#             */
/*   Updated: 2025/12/24 02:34:49 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATABASE_UTILITIES_HPP
#define DATABASE_UTILITIES_HPP

#include "database_system.hpp"
#include <functional>
#include <algorithm>
#include <numeric>
#include <cmath>

// ============================================================================
// ADVANCED QUERY UTILITIES
// ============================================================================

namespace Query {
    // Predicate-based filtering
    using RowPredicate = std::function<bool(const Row&)>;
    
    std::vector<Row> filter(const Table& table, RowPredicate pred) {
        std::vector<Row> results;
        for (const auto& row : table.rows()) {
            if (pred(row)) {
                results.push_back(row);
            }
        }
        return results;
    }
    
    // Column-based operations
    template<typename T>
    std::vector<T> selectColumn(const Table& table, const std::string& columnName,
                                std::function<T(const std::string&)> converter) {
        std::vector<T> values;
        for (const auto& row : table.rows()) {
            values.push_back(converter(row.getValue(columnName)));
        }
        return values;
    }
    
    // Aggregate functions
    double sum(const Table& table, const std::string& columnName) {
        auto values = selectColumn<double>(table, columnName, 
            [](const std::string& s) { return std::stod(s); });
        return std::accumulate(values.begin(), values.end(), 0.0);
    }
    
    double average(const Table& table, const std::string& columnName) {
        if (table.rowCount() == 0) return 0.0;
        return sum(table, columnName) / table.rowCount();
    }
    
    double min(const Table& table, const std::string& columnName) {
        auto values = selectColumn<double>(table, columnName,
            [](const std::string& s) { return std::stod(s); });
        return values.empty() ? 0.0 : *std::min_element(values.begin(), values.end());
    }
    
    double max(const Table& table, const std::string& columnName) {
        auto values = selectColumn<double>(table, columnName,
            [](const std::string& s) { return std::stod(s); });
        return values.empty() ? 0.0 : *std::max_element(values.begin(), values.end());
    }
    
    size_t count(const Table& table, RowPredicate pred) {
        return filter(table, pred).size();
    }
}

// ============================================================================
// TABLE TRANSFORMATION UTILITIES
// ============================================================================

namespace Transform {
    // Sort table by column
    Table sortBy(const Table& table, const std::string& columnName, bool ascending = true) {
        Table sorted;
        for (const auto& col : table.columns()) {
            sorted.addColumn(col);
        }
        
        std::vector<Row> rows = table.rows();
        std::sort(rows.begin(), rows.end(), [&](const Row& a, const Row& b) {
            std::string valA = a.getValue(columnName);
            std::string valB = b.getValue(columnName);
            return ascending ? (valA < valB) : (valA > valB);
        });
        
        for (const auto& row : rows) {
            sorted.addRow(row);
        }
        
        return sorted;
    }
    
    // Group by column value
    std::map<std::string, std::vector<Row>> groupBy(const Table& table, 
                                                     const std::string& columnName) {
        std::map<std::string, std::vector<Row>> groups;
        for (const auto& row : table.rows()) {
            groups[row.getValue(columnName)].push_back(row);
        }
        return groups;
    }
    
    // Select specific columns
    Table selectColumns(const Table& table, const std::vector<std::string>& columnNames) {
        Table result;
        
        // Add only selected columns
        for (const auto& col : table.columns()) {
            if (std::find(columnNames.begin(), columnNames.end(), col.name()) 
                != columnNames.end()) {
                result.addColumn(col);
            }
        }
        
        // Add rows with only selected columns
        for (const auto& row : table.rows()) {
            Row newRow;
            for (const auto& colName : columnNames) {
                newRow.setValue(colName, row.getValue(colName));
            }
            result.addRow(newRow);
        }
        
        return result;
    }
    
    // Limit number of rows
    Table limit(const Table& table, size_t maxRows) {
        Table result;
        for (const auto& col : table.columns()) {
            result.addColumn(col);
        }
        
        size_t count = 0;
        for (const auto& row : table.rows()) {
            if (count++ >= maxRows) break;
            result.addRow(row);
        }
        
        return result;
    }
}

// ============================================================================
// DATA GENERATION UTILITIES
// ============================================================================

namespace Generator {
    // Generate sample data
    Database createSampleEmployees(size_t count = 10) {
        Database db;
        
        db.addColumn("ID", ColumnType::INTEGER, Alignment::RIGHT);
        db.addColumn("Name", ColumnType::STRING, Alignment::LEFT);
        db.addColumn("Dept", ColumnType::STRING, Alignment::LEFT);
        db.addColumn("Salary", ColumnType::INTEGER, Alignment::RIGHT);
        db.addColumn("Active", ColumnType::BOOLEAN, Alignment::CENTER);
        
        std::vector<std::string> names = {
            "Alice", "Bob", "Charlie", "Diana", "Eve", "Frank", 
            "Grace", "Henry", "Iris", "Jack"
        };
        std::vector<std::string> depts = {
            "Engineering", "Marketing", "Sales", "HR", "Finance"
        };
        
        for (size_t i = 0; i < count && i < names.size(); ++i) {
            db.addRow({
                {"ID", std::to_string(i + 1)},
                {"Name", names[i]},
                {"Dept", depts[i % depts.size()]},
                {"Salary", std::to_string(50000 + (i * 5000))},
                {"Active", (i % 3 != 0) ? "true" : "false"}
            });
        }
        
        return db;
    }
    
    // Generate sample sales data
    Database createSampleSales(size_t count = 8) {
        Database db;
        
        db.addColumn("Month", ColumnType::STRING, Alignment::LEFT);
        db.addColumn("Product", ColumnType::STRING, Alignment::LEFT);
        db.addColumn("Units", ColumnType::INTEGER, Alignment::RIGHT);
        db.addColumn("Revenue", ColumnType::STRING, Alignment::RIGHT);
        
        std::vector<std::string> months = {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug"
        };
        std::vector<std::string> products = {"Widget A", "Widget B", "Widget C"};
        
        for (size_t i = 0; i < count && i < months.size(); ++i) {
            db.addRow({
                {"Month", months[i]},
                {"Product", products[i % products.size()]},
                {"Units", std::to_string(100 + (i * 25))},
                {"Revenue", "$" + std::to_string((100 + i * 25) * 50)}
            });
        }
        
        return db;
    }
}

// ============================================================================
// STYLE PRESETS
// ============================================================================

namespace StylePresets {
    RenderConfig matrix() {
        RenderConfig cfg;
        cfg.boxChars = Unicode::BoxChars::heavy();
        cfg.headerStyle.foreground = Style::Color::BrightGreen();
        cfg.headerStyle.bold = true;
        cfg.cellStyle.foreground = Style::Color::Green();
        cfg.padding = 1;
        return cfg;
    }
    
    RenderConfig ocean() {
        RenderConfig cfg;
        cfg.boxChars = Unicode::BoxChars::rounded();
        cfg.headerStyle.foreground = Style::Color::BrightCyan();
        cfg.headerStyle.bold = true;
        cfg.cellStyle.foreground = Style::Color::Cyan();
        cfg.padding = 2;
        return cfg;
    }
    
    RenderConfig fire() {
        RenderConfig cfg;
        cfg.boxChars = Unicode::BoxChars::doubleLine();
        cfg.headerStyle.foreground = Style::Color::BrightRed();
        cfg.headerStyle.bold = true;
        cfg.cellStyle.foreground = Style::Color::BrightYellow();
        cfg.padding = 1;
        return cfg;
    }
    
    RenderConfig corporate() {
        RenderConfig cfg;
        cfg.boxChars = Unicode::BoxChars::doubleLine();
        cfg.headerStyle.foreground = Style::Color::BrightWhite();
        cfg.headerStyle.background = Style::Color{44}; // Blue bg
        cfg.headerStyle.bold = true;
        cfg.padding = 2;
        cfg.showFooter = false;
        return cfg;
    }
    
    RenderConfig newspaper() {
        RenderConfig cfg;
        cfg.headerStyle.foreground = Style::Color::Default();
        cfg.headerStyle.bold = true;
        cfg.headerStyle.underline = true;
        cfg.cellStyle.foreground = Style::Color::Default();
        cfg.padding = 1;
        return cfg;
    }
}

// ============================================================================
// EXPORT UTILITIES
// ============================================================================

namespace Export {
    // Export to CSV
    void toCsv(const Table& table, const std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot create file: " + path);
        }
        
        // Write header
        const auto& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            file << cols[i].name();
            if (i < cols.size() - 1) file << ",";
        }
        file << "\n";
        
        // Write rows
        for (const auto& row : table.rows()) {
            for (size_t i = 0; i < cols.size(); ++i) {
                std::string value = row.getValue(cols[i].name());
                // Escape if contains comma or quote
                if (value.find(',') != std::string::npos || 
                    value.find('"') != std::string::npos) {
                    value = "\"" + value + "\"";
                }
                file << value;
                if (i < cols.size() - 1) file << ",";
            }
            file << "\n";
        }
        
        file.close();
    }
    
    // Export to HTML
    std::string toHtml(Table& table, const std::string& title = "Data Table") {
        std::ostringstream html;
        
        html << "<!DOCTYPE html>\n<html>\n<head>\n";
        html << "<title>" << title << "</title>\n";
        html << "<style>\n";
        html << "body { font-family: Arial, sans-serif; margin: 20px; }\n";
        html << "table { border-collapse: collapse; width: 100%; }\n";
        html << "th, td { border: 1px solid #ddd; padding: 12px; text-align: left; }\n";
        html << "th { background-color: #4CAF50; color: white; font-weight: bold; }\n";
        html << "tr:nth-child(even) { background-color: #f2f2f2; }\n";
        html << "tr:hover { background-color: #ddd; }\n";
        html << "</style>\n</head>\n<body>\n";
        html << "<h1>" << title << "</h1>\n";
        html << "<table>\n<thead>\n<tr>\n";
        
        // Header
        for (const auto& col : table.columns()) {
            html << "<th>" << col.name() << "</th>\n";
        }
        html << "</tr>\n</thead>\n<tbody>\n";
        
        // Rows
        for (const auto& row : table.rows()) {
            html << "<tr>\n";
            for (const auto& col : table.columns()) {
                html << "<td>" << row.getValue(col.name()) << "</td>\n";
            }
            html << "</tr>\n";
        }
        
        html << "</tbody>\n</table>\n</body>\n</html>";
        return html.str();
    }
    
    // Export to Markdown
    std::string toMarkdown(Table& table) {
        table.calculateColumnWidths();
        std::ostringstream md;
        
        const auto& cols = table.columns();
        
        // Header row
        for (const auto& col : cols) {
            md << "| " << Unicode::pad(col.name(), col.width(), 'l') << " ";
        }
        md << "|\n";
        
        // Separator
        for (const auto& col : cols) {
            md << "|" << std::string(col.width() + 2, '-');
        }
        md << "|\n";
        
        // Data rows
        for (const auto& row : table.rows()) {
            for (const auto& col : cols) {
                std::string value = row.getValue(col.name());
                md << "| " << Unicode::pad(value, col.width(), col.getAlignChar()) << " ";
            }
            md << "|\n";
        }
        
        return md.str();
    }
}

// ============================================================================
// STATISTICS UTILITIES
// ============================================================================

namespace Statistics {
    struct ColumnStats {
        double min;
        double max;
        double mean;
        double median;
        double stdDev;
        size_t count;
    };
    
    ColumnStats analyze(const Table& table, const std::string& columnName) {
        ColumnStats stats = {0, 0, 0, 0, 0, 0};
        
        auto values = Query::selectColumn<double>(table, columnName,
            [](const std::string& s) { 
                try { return std::stod(s); } 
                catch (...) { return 0.0; }
            });
        
        if (values.empty()) return stats;
        
        stats.count = values.size();
        stats.min = *std::min_element(values.begin(), values.end());
        stats.max = *std::max_element(values.begin(), values.end());
        stats.mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        
        // Median
        std::vector<double> sorted = values;
        std::sort(sorted.begin(), sorted.end());
        stats.median = sorted[sorted.size() / 2];
        
        // Standard deviation
        double variance = 0.0;
        for (double v : values) {
            variance += (v - stats.mean) * (v - stats.mean);
        }
        stats.stdDev = std::sqrt(variance / values.size());
        
        return stats;
    }
    
    Database createStatsTable(const ColumnStats& stats, const std::string& columnName) {
        Database db;
        
        db.addColumn("Statistic", ColumnType::STRING, Alignment::LEFT);
        db.addColumn("Value", ColumnType::STRING, Alignment::RIGHT);
        
        db.addRow({{"Statistic", "Column"}, {"Value", columnName}});
        db.addRow({{"Statistic", "Count"}, {"Value", std::to_string(stats.count)}});
        db.addRow({{"Statistic", "Min"}, {"Value", std::to_string(stats.min)}});
        db.addRow({{"Statistic", "Max"}, {"Value", std::to_string(stats.max)}});
        db.addRow({{"Statistic", "Mean"}, {"Value", std::to_string(stats.mean)}});
        db.addRow({{"Statistic", "Median"}, {"Value", std::to_string(stats.median)}});
        db.addRow({{"Statistic", "Std Dev"}, {"Value", std::to_string(stats.stdDev)}});
        
        return db;
    }
}

#endif // DATABASE_UTILITIES_HPP