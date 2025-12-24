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
#define DATABASE_HPP

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>

// ============================================================================
// UNICODE UTILITIES
// ============================================================================

namespace Unicode {
    struct BoxChars {
        std::string topLeft;
        std::string topRight;
        std::string bottomLeft;
        std::string bottomRight;
        std::string horizontal;
        std::string vertical;
        std::string cross;
        std::string teeLeft;
        std::string teeRight;
        std::string teeTop;
        std::string teeBottom;

        BoxChars() {
            topLeft = "┌"; topRight = "┐"; bottomLeft = "└"; bottomRight = "┘";
            horizontal = "─"; vertical = "│"; cross = "┼";
            teeLeft = "├"; teeRight = "┤"; teeTop = "┬"; teeBottom = "┴";
        }

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
    struct Color {
        int code;
        Color() : code(39) {}
        explicit Color(int c) : code(c) {}
        static Color Black() { return Color(30); }
        static Color Red() { return Color(31); }
        static Color Green() { return Color(32); }
        static Color Yellow() { return Color(33); }
        static Color Blue() { return Color(34); }
        static Color Magenta() { return Color(35); }
        static Color Cyan() { return Color(36); }
        static Color White() { return Color(37); }
        static Color BrightBlack() { return Color(90); }
        static Color BrightRed() { return Color(91); }
        static Color BrightGreen() { return Color(92); }
        static Color BrightYellow() { return Color(93); }
        static Color BrightBlue() { return Color(94); }
        static Color BrightMagenta() { return Color(95); }
        static Color BrightCyan() { return Color(96); }
        static Color BrightWhite() { return Color(97); }
        static Color Default() { return Color(39); }

        static Color RGB(int r, int g, int b) {
            return Color(30 + (r > 127 ? 0 : 0) + (g > 127 ? 2 : 0) + (b > 127 ? 4 : 0));
        }
    };

    struct CellStyle {
        Color foreground;
        Color background;
        bool bold;
        bool italic;
        bool underline;

        CellStyle() : foreground(Color::Default()), background(Color(49)), bold(false), italic(false), underline(false) {}

        std::string apply(const std::string& text) const {
            std::ostringstream oss;
            oss << "\033[";
            bool first = true;

            if (bold) { if (!first) oss << ";"; oss << "1"; first = false; }
            if (italic) { if (!first) oss << ";"; oss << "3"; first = false; }
            if (underline) { if (!first) oss << ";"; oss << "4"; first = false; }
            if (foreground.code != 39) { if (!first) oss << ";"; oss << foreground.code; first = false; }
            if (background.code != 49) { if (!first) oss << ";"; oss << background.code; first = false; }

            oss << "m" << text << "\033[0m";
            return oss.str();
        }
    };
}

// ============================================================================
// COLUMN TYPE SYSTEM
// ============================================================================

/* replace enum class (C++11) with plain enums */
namespace ColumnType {
    enum Type { STRING = 0, INTEGER = 1, DOUBLE = 2, DATE = 3, BOOLEAN = 4 };
}

namespace Alignment {
    enum Type { LEFT = 0, CENTER = 1, RIGHT = 2 };
}

class Column {
public:
    Column(const std::string& name, ColumnType::Type type = ColumnType::STRING, Alignment::Type align = Alignment::LEFT)
        : _name(name), _type(type), _alignment(align), _width(0) {}

    const std::string& name() const { return _name; }
    ColumnType::Type type() const { return _type; }
    Alignment::Type alignment() const { return _alignment; }
    size_t width() const { return _width; }
    void setWidth(size_t w) { _width = w; }

    char getAlignChar() const {
        if (_alignment == Alignment::CENTER) return 'c';
        if (_alignment == Alignment::RIGHT) return 'r';
        return 'l';
    }

    std::string format(const std::string& value) const {
        if (_type == ColumnType::INTEGER || _type == ColumnType::DOUBLE) return value;
        if (_type == ColumnType::BOOLEAN) return (value == "1" || value == "true") ? "✓" : "✗";
        return value;
    }

private:
    std::string _name;
    ColumnType::Type _type;
    Alignment::Type _alignment;
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
        std::map<std::string, std::string>::const_iterator it = _data.find(columnName);
        if (it != _data.end()) return it->second;
        return "";
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
        for (size_t ci = 0; ci < _columns.size(); ++ci) {
            Column& col = _columns[ci];
            size_t maxWidth = Unicode::displayWidth(col.name());

            for (size_t ri = 0; ri < _rows.size(); ++ri) {
                const Row& row = _rows[ri];
                std::string value = col.format(row.getValue(col.name()));
                size_t width = Unicode::displayWidth(value);
                if (width > maxWidth) maxWidth = width;
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
    bool showHeader;
    bool showFooter;
    std::string footerText;
    int padding;
    bool autoWidth;

    RenderConfig() : boxChars(), headerStyle(), cellStyle(), footerStyle(), showHeader(true),
                     showFooter(false), footerText(""), padding(1), autoWidth(true) {
        headerStyle.bold = true;
        headerStyle.foreground = Style::Color::BrightCyan();
    }

    static RenderConfig elegant() {
        RenderConfig cfg;
        cfg.boxChars = Unicode::BoxChars::doubleLine();
        cfg.headerStyle.bold = true;
        cfg.headerStyle.foreground = Style::Color::BrightWhite();
        cfg.headerStyle.background = Style::Color(44);
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

        const std::vector<Column>& cols = table.columns();
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

        const std::vector<Column>& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            std::string content = Unicode::pad(cols[i].name(), cols[i].width(), 'c');

            _buffer << repeat(" ", _config.padding);
            _buffer << _config.headerStyle.apply(content);
            _buffer << repeat(" ", _config.padding);
            _buffer << _config.boxChars.vertical;
        }

        _buffer << "\n";
    }

    void renderHeaderSeparator(const Table& table) {
        _buffer << _config.boxChars.teeLeft;

        const std::vector<Column>& cols = table.columns();
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
        const std::vector<Column>& cols = table.columns();
        const std::vector<Row>& rows = table.rows();

        for (size_t r = 0; r < rows.size(); ++r) {
            _buffer << _config.boxChars.vertical;

            for (size_t c = 0; c < cols.size(); ++c) {
                std::string value = cols[c].format(rows[r].getValue(cols[c].name()));
                std::string content = Unicode::pad(value, cols[c].width(), cols[c].getAlignChar());

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

        const std::vector<Column>& cols = table.columns();
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
        const std::vector<Column>& cols = table.columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            totalWidth += cols[i].width() + 2 * _config.padding + 1;
        }
        totalWidth -= 1;

        std::string content = Unicode::pad(_config.footerText, totalWidth - 2, 'c');
        _buffer << " " << _config.footerStyle.apply(content) << " ";
        _buffer << _config.boxChars.vertical << "\n";
    }
    
    void renderBottomBorder(const Table& table) {
        _buffer << _config.boxChars.bottomLeft;

        const std::vector<Column>& cols = table.columns();
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
        std::ifstream file;
        file.open(path.c_str());
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + path);
        }

        std::string line;
        bool isFirstLine = true;
        std::vector<std::string> headers;

        while (std::getline(file, line)) {
            std::vector<std::string> fields = parseLine(line);

            if (isFirstLine && hasHeader) {
                headers = fields;
                for (size_t i = 0; i < headers.size(); ++i) {
                    table.addColumn(Column(headers[i]));
                }
                isFirstLine = false;
            } else {
                if (headers.empty()) {
                    for (size_t i = 0; i < fields.size(); ++i) {
                        std::ostringstream oss;
                        oss << (i + 1);
                        headers.push_back(std::string("Column") + oss.str());
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
 
    void addColumn(const std::string& name, ColumnType::Type type = ColumnType::STRING,
                   Alignment::Type align = Alignment::LEFT) {
        _table.addColumn(Column(name, type, align));
    }
 
    void addRow(const std::map<std::string, std::string>& data) {
        Row row;
        for (std::map<std::string, std::string>::const_iterator it = data.begin(); it != data.end(); ++it) {
            row.setValue(it->first, it->second);
        }
        _table.addRow(row);
    }
 
    std::string render(const RenderConfig& config = RenderConfig()) {
        TableRenderer renderer(config);
        return renderer.render(_table);
    }
 
    Table& table() { return _table; }
    const Table& table() const { return _table; }
 
    std::vector<Row> where(const std::string& column, const std::string& value) const {
        std::vector<Row> results;
        const std::vector<Row>& rows = _table.rows();
        for (size_t i = 0; i < rows.size(); ++i) {
            if (rows[i].getValue(column) == value) {
                results.push_back(rows[i]);
            }
        }
        return results;
    }
 
    size_t count() const { return _table.rowCount(); }

private:
    Table _table;
};

#endif // DATABASE_HPP