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

#include <stdint.h>

#include <algorithm>
#include <cctype>
#include <clocale>
#include <cwchar>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "ft_string.hpp"  // use Unicode-aware case helpers from ft_string.cpp

// Forward declare strcase_toggle (defined in ft_string.cpp)
void strcase_toggle(std::string* s, int mod);

// ============================================================================
// UNICODE UTILITIES
// ============================================================================

struct Unicode {
  // Decode a single UTF-8 character and return its code point
  static uint32_t decodeUTF8(const std::string& str, size_t& index) {
    uint32_t codepoint = 0;
    unsigned char c = (unsigned char)str[index];

    if ((c & 0x80) == 0) {
      // 1-byte ASCII
      codepoint = c;
      index += 1;
    } else if ((c & 0xE0) == 0xC0) {
      // 2-byte sequence
      if (index + 1 < str.size()) {
        codepoint = ((c & 0x1F) << 6) | ((unsigned char)str[index + 1] & 0x3F);
      }
      index += 2;
    } else if ((c & 0xF0) == 0xE0) {
      // 3-byte sequence
      if (index + 2 < str.size()) {
        codepoint = ((c & 0x0F) << 12) |
                    (((unsigned char)str[index + 1] & 0x3F) << 6) |
                    ((unsigned char)str[index + 2] & 0x3F);
      }
      index += 3;
    } else if ((c & 0xF8) == 0xF0) {
      // 4-byte sequence (emoji, etc.)
      if (index + 3 < str.size()) {
        codepoint = ((c & 0x07) << 18) |
                    (((unsigned char)str[index + 1] & 0x3F) << 12) |
                    (((unsigned char)str[index + 2] & 0x3F) << 6) |
                    ((unsigned char)str[index + 3] & 0x3F);
      }
      index += 4;
    } else {
      // Invalid sequence, skip
      index += 1;
    }

    return codepoint;
  }

  // Ensure locale is set (for wcwidth to work correctly)
  static void _initLocale() {
    static bool done = false;
    if (done) return;
    done = true;
    std::setlocale(LC_CTYPE, "");
  }

  // ── Single-codepoint display width via system wcwidth() ─────────────
  // Delegates entirely to the C library so we match the terminal's own
  // cursor advancement.  Only clamps negative returns (unknown codepoints)
  // to zero.
  static int codepointWidth(uint32_t cp) {
    _initLocale();
    int w = wcwidth(static_cast<wchar_t>(cp));
    return w > 0 ? w : 0;
  }

  // ── Helpers for displayWidth state machine ──────────────────────────

  // Is this codepoint a regional indicator letter (A-Z for flags)?
  static bool _isRegionalIndicator(uint32_t cp) {
    return cp >= 0x1F1E6 && cp <= 0x1F1FF;
  }

  // Calculate display width of a UTF-8 string.
  // Uses system wcwidth() for individual codepoint widths, which matches
  // what the terminal uses for cursor advancement.  A minimal state
  // machine handles multi-codepoint emoji clusters (ZWJ sequences,
  // regional-indicator flag pairs, skin-tone modifiers, tag characters)
  // that the terminal renders as a single 2-column glyph.
  static size_t displayWidth(const std::string& str) {
    size_t width = 0;
    size_t index = 0;
    bool inZwjSeq = false;  // currently inside a ZWJ sequence
    int regionalCount = 0;  // count of consecutive regional indicators

    while (index < str.size()) {
      uint32_t cp = decodeUTF8(str, index);
      if (cp == 0) continue;

      // ── Regional Indicator pairs (flags) ─────────────────────────
      if (_isRegionalIndicator(cp)) {
        ++regionalCount;
        if (regionalCount == 1) {
          width += 2;  // first indicator: reserve 2 columns
        } else if (regionalCount == 2) {
          regionalCount = 0;  // second indicator: already counted
        }
        continue;
      }
      regionalCount = 0;

      // ── Zero Width Joiner ────────────────────────────────────────
      if (cp == 0x200D) {
        inZwjSeq = true;
        continue;
      }

      // ── Skin tone modifiers (absorbed into preceding emoji) ──────
      if (cp >= 0x1F3FB && cp <= 0x1F3FF) continue;

      // ── Tag characters (subdivision flags) ───────────────────────
      if ((cp >= 0xE0020 && cp <= 0xE007F) || cp == 0xE0001) continue;

      // ── Emoji after ZWJ: absorbed into the ZWJ sequence ──────────
      if (inZwjSeq) {
        inZwjSeq = false;
        continue;
      }

      // ── Regular codepoint — use system wcwidth ───────────────────
      width += static_cast<size_t>(codepointWidth(cp));
    }

    return width;
  }

  // Pad string to specific display width
  static std::string pad(const std::string& str, size_t width,
                         char align = 'l') {
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
      topLeft = "┌";
      topRight = "┐";
      bottomLeft = "└";
      bottomRight = "┘";
      horizontal = "─";
      vertical = "│";
      cross = "┼";
      teeLeft = "├";
      teeRight = "┤";
      teeTop = "┬";
      teeBottom = "┴";
    }

    static BoxChars heavy() {
      BoxChars b;
      b.topLeft = "┏";
      b.topRight = "┓";
      b.bottomLeft = "┗";
      b.bottomRight = "┛";
      b.horizontal = "━";
      b.vertical = "┃";
      b.cross = "╋";
      b.teeLeft = "┣";
      b.teeRight = "┫";
      b.teeTop = "┳";
      b.teeBottom = "┻";
      return b;
    }

    static BoxChars doubleLine() {
      BoxChars b;
      b.topLeft = "╔";
      b.topRight = "╗";
      b.bottomLeft = "╚";
      b.bottomRight = "╝";
      b.horizontal = "═";
      b.vertical = "║";
      b.cross = "╬";
      b.teeLeft = "╠";
      b.teeRight = "╣";
      b.teeTop = "╦";
      b.teeBottom = "╩";
      return b;
    }

    static BoxChars rounded() {
      BoxChars b;
      b.topLeft = "╭";
      b.topRight = "╮";
      b.bottomLeft = "╰";
      b.bottomRight = "╯";
      return b;
    }
  };
};  // struct Unicode

// ============================================================================
// COLOR AND STYLE UTILITIES
// ============================================================================

struct Style {
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
      return Color(30 + (r > 127 ? 0 : 0) + (g > 127 ? 2 : 0) +
                   (b > 127 ? 4 : 0));
    }
  };

  struct CellStyle {
    Color foreground;
    Color background;
    bool bold;
    bool italic;
    bool underline;

    CellStyle()
        : foreground(Color::Default()),
          background(Color(49)),
          bold(false),
          italic(false),
          underline(false) {}

    std::string apply(const std::string& text) const {
      std::ostringstream oss;
      oss << "\033[";
      bool first = true;

      if (bold) {
        if (!first) oss << ";";
        oss << "1";
        first = false;
      }
      if (italic) {
        if (!first) oss << ";";
        oss << "3";
        first = false;
      }
      if (underline) {
        if (!first) oss << ";";
        oss << "4";
        first = false;
      }
      if (foreground.code != 39) {
        if (!first) oss << ";";
        oss << foreground.code;
        first = false;
      }
      if (background.code != 49) {
        if (!first) oss << ";";
        oss << background.code;
        first = false;
      }

      oss << "m" << text << "\033[0m";
      return oss.str();
    }
  };
};  // struct Style

// ============================================================================
// COLUMN TYPE SYSTEM
// ============================================================================

struct ColumnType {
  enum Type { STRING = 0, INTEGER = 1, DOUBLE = 2, DATE = 3, BOOLEAN = 4 };
};

struct Alignment {
  enum Type { LEFT = 0, CENTER = 1, RIGHT = 2 };
};

class Column {
 public:
  Column(const std::string& name, ColumnType::Type type = ColumnType::STRING,
         Alignment::Type align = Alignment::LEFT)
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
    if (_type == ColumnType::INTEGER || _type == ColumnType::DOUBLE)
      return value;
    if (_type == ColumnType::BOOLEAN)
      return (value == "1" || value == "true") ? "✓" : "✗";
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
    std::map<std::string, std::string>::const_iterator it =
        _data.find(columnName);
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

  void addColumn(const Column& col) { _columns.push_back(col); }

  void addRow(const Row& row) { _rows.push_back(row); }

  const std::vector<Column>& columns() const { return _columns; }
  std::vector<Column>& columns() { return _columns; }
  const std::vector<Row>& rows() const { return _rows; }
  // NEW: non-const rows accessor so callers can modify rows (REPL/queries)
  std::vector<Row>& rows() { return _rows; }

  void clear() { _rows.clear(); }

  size_t columnCount() const { return _columns.size(); }
  size_t rowCount() const { return _rows.size(); }

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

enum TableStyle {
  TABLE_STYLE_LIGHT = 0,
  TABLE_STYLE_HEAVY,
  TABLE_STYLE_DOUBLE,
  TABLE_STYLE_ROUNDED,
  TABLE_STYLE_MINIMAL
};

struct RenderConfig {
  Unicode::BoxChars boxChars;
  Style::CellStyle headerStyle;
  Style::CellStyle cellStyle;
  Style::CellStyle footerStyle;
  Style::CellStyle borderStyle;  // new: style for borders / box chars
  bool showHeader;
  bool showFooter;
  std::string footerText;
  int padding;
  bool autoWidth;

  // NEW: auto-increment ID behavior when loading CSV
  bool autoIncrementId;

  // NEW: body coloring options
  Style::Color
      body_background;      // background applied to all body cells (if != 49)
  Style::Color color_body;  // foreground applied to all body cells (if != 39)
  Style::Color even_background;  // background for even rows (overrides
                                 // body_background if set)
  Style::Color odd_background;   // background for odd rows
  std::map<std::string, Style::Color>
      color_column;  // per-column foreground overrides (column name -> color)
  std::map<std::string, Style::CellStyle>
      value_styles;       // value-based cell rules
  TableStyle tableStyle;  // new: choose shape

  RenderConfig()
      : boxChars(),
        headerStyle(),
        cellStyle(),
        footerStyle(),
        showHeader(true),
        showFooter(false),
        footerText(""),
        padding(1),
        autoWidth(true),
        autoIncrementId(true),
        body_background(Style::Color(49)),
        color_body(Style::Color::Default()),
        even_background(Style::Color(49)),
        odd_background(Style::Color(49)),
        tableStyle(TABLE_STYLE_ROUNDED)  // default to rounded corners
  {
    headerStyle.bold = true;
    headerStyle.foreground = Style::Color::BrightCyan();
    borderStyle.foreground = Style::Color::BrightWhite();
    // default box chars follow the default table style
    boxChars = Unicode::BoxChars::rounded();
  }

  static RenderConfig elegant() {
    RenderConfig cfg;
    cfg.boxChars = Unicode::BoxChars::doubleLine();
    cfg.headerStyle.bold = true;
    cfg.headerStyle.foreground = Style::Color::BrightWhite();
    cfg.headerStyle.background = Style::Color(44);
    cfg.borderStyle.foreground = Style::Color::BrightWhite();
    cfg.tableStyle =
        TABLE_STYLE_DOUBLE;  // ensure double lines for elegant style
    return cfg;
  }

  static RenderConfig minimal() {
    RenderConfig cfg;
    cfg.padding = 0;
    cfg.headerStyle.bold = true;
    cfg.borderStyle.foreground = Style::Color::Default();
    cfg.tableStyle = TABLE_STYLE_MINIMAL;  // ensure minimal style
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

    // If a footer is shown, ensure the table is wide enough to contain it.
    // Expand the last column if the footer text is wider than the total inner
    // width.
    if (_config.showFooter && !_config.footerText.empty()) {
      std::vector<Column>& cols = table.columns();
      size_t totalInner = 0;
      for (size_t i = 0; i < cols.size(); ++i) {
        totalInner += cols[i].width() + 2 * _config.padding;
        if (i + 1 < cols.size()) totalInner += 1;
      }
      // +2 accounts for the 1-char padding on each side inside the merged
      // footer cell
      size_t needed = Unicode::displayWidth(_config.footerText) + 2;
      if (needed > totalInner && !cols.empty()) {
        size_t deficit = needed - totalInner;
        cols.back().setWidth(cols.back().width() + deficit);
      }
    }

    _buffer.clear();
    _buffer.str("");

    // Top/header/data rendering
    renderTopBorder(table);
    if (_config.showHeader) {
      renderHeader(table);
      renderHeaderSeparator(table);
    }
    renderRows(table);

    if (_config.showFooter && !_config.footerText.empty()) {
      renderFooterSeparator(table);
      renderFooter(table);
      renderBottomBorder(table, true);
    } else {
      renderBottomBorder(table, false);
    }

    return _buffer.str();
  }

  void setConfig(const RenderConfig& config) { _config = config; }

 private:
  RenderConfig _config;
  std::ostringstream _buffer;

  // helper to output a box character (styled)
  void putBorder(const std::string& s) {
    // apply border style around the given string
    _buffer << _config.borderStyle.apply(s);
  }

  void renderTopBorder(const Table& table) {
    putBorder(_config.boxChars.topLeft);

    const std::vector<Column>& cols = table.columns();
    for (size_t i = 0; i < cols.size(); ++i) {
      size_t width = cols[i].width() + 2 * _config.padding;
      putBorder(repeat(_config.boxChars.horizontal, width));

      if (i < cols.size() - 1) {
        putBorder(_config.boxChars.teeTop);
      }
    }

    putBorder(_config.boxChars.topRight);
    _buffer << "\n";
  }

  void renderHeader(const Table& table) {
    putBorder(_config.boxChars.vertical);

    const std::vector<Column>& cols = table.columns();
    for (size_t i = 0; i < cols.size(); ++i) {
      std::string content = Unicode::pad(cols[i].name(), cols[i].width(), 'c');

      _buffer << repeat(" ", _config.padding);
      _buffer << _config.headerStyle.apply(content);
      _buffer << repeat(" ", _config.padding);
      putBorder(_config.boxChars.vertical);
    }

    _buffer << "\n";
  }

  void renderHeaderSeparator(const Table& table) {
    putBorder(_config.boxChars.teeLeft);

    const std::vector<Column>& cols = table.columns();
    for (size_t i = 0; i < cols.size(); ++i) {
      size_t width = cols[i].width() + 2 * _config.padding;
      putBorder(repeat(_config.boxChars.horizontal, width));

      if (i < cols.size() - 1) {
        putBorder(_config.boxChars.cross);
      }
    }

    putBorder(_config.boxChars.teeRight);
    _buffer << "\n";
  }

  void renderRows(const Table& table) {
    const std::vector<Column>& cols = table.columns();
    const std::vector<Row>& rows = table.rows();

    for (size_t r = 0; r < rows.size(); ++r) {
      putBorder(_config.boxChars.vertical);

      for (size_t c = 0; c < cols.size(); ++c) {
        std::string raw = cols[c].format(rows[r].getValue(cols[c].name()));
        std::string content =
            Unicode::pad(raw, cols[c].width(), cols[c].getAlignChar());

        // Build effective cell style: start from default cellStyle
        Style::CellStyle eff = _config.cellStyle;

        // global body foreground override
        if (_config.color_body.code != Style::Color::Default().code) {
          eff.foreground = _config.color_body;
        }

        // per-column foreground override (higher priority)
        std::map<std::string, Style::Color>::const_iterator itcol =
            _config.color_column.find(cols[c].name());
        if (itcol != _config.color_column.end()) {
          eff.foreground = itcol->second;
        }

        // background: base is body_background unless row stripe overrides
        if (_config.body_background.code != 49) {
          eff.background = _config.body_background;
        }

        // row stripe overrides
        if ((r % 2) == 0) {  // even row (0-based)
          if (_config.even_background.code != 49)
            eff.background = _config.even_background;
        } else {
          if (_config.odd_background.code != 49)
            eff.background = _config.odd_background;
        }

        // NEW: value-based styling (case-insensitive)
        std::string key = raw;
        strcase_toggle(&key, 1);  // lowercase
        std::map<std::string, Style::CellStyle>::const_iterator itv =
            _config.value_styles.find(key);
        if (itv != _config.value_styles.end()) {
          const Style::CellStyle& rule = itv->second;
          // override foreground if provided (non-default)
          if (rule.foreground.code != Style::Color::Default().code)
            eff.foreground = rule.foreground;
          // override background if provided (non-default)
          if (rule.background.code != 49) eff.background = rule.background;
          // override text attributes if requested (true means enable)
          if (rule.bold) eff.bold = true;
          if (rule.italic) eff.italic = true;
          if (rule.underline) eff.underline = true;
        }

        _buffer << repeat(" ", _config.padding);
        _buffer << eff.apply(content);
        _buffer << repeat(" ", _config.padding);
        putBorder(_config.boxChars.vertical);
      }

      _buffer << "\n";
    }
  }

  void renderFooterSeparator(const Table& table) {
    putBorder(_config.boxChars.teeLeft);

    const std::vector<Column>& cols = table.columns();
    for (size_t i = 0; i < cols.size(); ++i) {
      size_t width = cols[i].width() + 2 * _config.padding;
      putBorder(repeat(_config.boxChars.horizontal, width));

      if (i < cols.size() - 1) {
        // Use teeBottom (┴) instead of cross (┼) because
        // the footer merges all columns into a single span.
        putBorder(_config.boxChars.teeBottom);
      }
    }

    putBorder(_config.boxChars.teeRight);
    _buffer << "\n";
  }

  void renderFooter(const Table& table) {
    putBorder(_config.boxChars.vertical);

    size_t totalWidth = 0;
    const std::vector<Column>& cols = table.columns();
    for (size_t i = 0; i < cols.size(); ++i) {
      totalWidth += cols[i].width() + 2 * _config.padding + 1;
    }
    totalWidth -= 1;

    std::string content = Unicode::pad(_config.footerText, totalWidth - 2, 'c');
    _buffer << " " << _config.footerStyle.apply(content) << " ";
    putBorder(_config.boxChars.vertical);
    _buffer << "\n";
  }

  void renderBottomBorder(const Table& table, bool afterFooter) {
    putBorder(_config.boxChars.bottomLeft);

    const std::vector<Column>& cols = table.columns();
    for (size_t i = 0; i < cols.size(); ++i) {
      size_t width = cols[i].width() + 2 * _config.padding;
      putBorder(repeat(_config.boxChars.horizontal, width));

      if (i < cols.size() - 1) {
        // After a merged footer row there are no column divisions,
        // so just continue the horizontal line instead of placing a tee.
        if (afterFooter)
          putBorder(_config.boxChars.horizontal);
        else
          putBorder(_config.boxChars.teeBottom);
      }
    }

    putBorder(_config.boxChars.bottomRight);
    _buffer << "\n";
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
  static Table parse(const std::string& path, bool hasHeader = true,
                     const RenderConfig& cfg = RenderConfig()) {
    Table table;
    std::ifstream file;
    file.open(path.c_str());
    if (!file.is_open()) {
      throw std::runtime_error("Cannot open file: " + path);
    }

    std::string line;
    bool isFirstLine = true;
    std::vector<std::string> headers;

    bool insertedIdColumn = false;
    size_t nextAutoId = 1;

    while (std::getline(file, line)) {
      std::vector<std::string> fields = parseLine(line);

      if (isFirstLine && hasHeader) {
        headers = fields;
        // detect existing ID header (case-insensitive, Unicode-aware)
        bool hasId = false;
        for (size_t i = 0; i < headers.size(); ++i) {
          std::string htmp = headers[i];
          strcase_toggle(&htmp, 1);  // to lower (mod==1)
          if (htmp == "id") {
            hasId = true;
            break;
          }
        }
        // if requested, auto-insert ID as first column
        if (cfg.autoIncrementId && !hasId) {
          headers.insert(headers.begin(), std::string("ID"));
          insertedIdColumn = true;
        }
        for (size_t i = 0; i < headers.size(); ++i) {
          table.addColumn(Column(headers[i]));
        }
        isFirstLine = false;
      } else {
        if (headers.empty()) {
          // no header provided: create Column1..N; optionally prepend ID
          for (size_t i = 0; i < fields.size(); ++i) {
            std::ostringstream oss;
            oss << (i + 1);
            headers.push_back(std::string("Column") + oss.str());
          }
          if (cfg.autoIncrementId) {
            headers.insert(headers.begin(), std::string("ID"));
            insertedIdColumn = true;
          }
          for (size_t i = 0; i < headers.size(); ++i) {
            table.addColumn(Column(headers[i]));
          }
        }

        Row row;
        size_t fi = 0;
        if (insertedIdColumn) {
          std::ostringstream idss;
          idss << nextAutoId++;
          row.setValue(headers[0], idss.str());
          // fill fields starting at headers[1]
          for (size_t i = 1; i < headers.size() && fi < fields.size();
               ++i, ++fi) {
            row.setValue(headers[i], fields[fi]);
          }
        } else {
          for (size_t i = 0; i < fields.size() && i < headers.size(); ++i) {
            row.setValue(headers[i], fields[i]);
          }
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

  void loadFromCsv(const std::string& path, bool hasHeader = true,
                   const RenderConfig& cfg = RenderConfig()) {
    _table = CsvParser::parse(path, hasHeader, cfg);
  }

  void addColumn(const std::string& name,
                 ColumnType::Type type = ColumnType::STRING,
                 Alignment::Type align = Alignment::LEFT) {
    _table.addColumn(Column(name, type, align));
  }

  void addRow(const std::map<std::string, std::string>& data) {
    Row row;
    for (std::map<std::string, std::string>::const_iterator it = data.begin();
         it != data.end(); ++it) {
      row.setValue(it->first, it->second);
    }
    _table.addRow(row);
  }

  // expose table for callers (needed by tests/util modules)
  Table& table() { return _table; }
  const Table& table() const { return _table; }

  // Render the table using TableRenderer so styles are always applied
  std::string render(const RenderConfig& config = RenderConfig()) const {
    TableRenderer renderer(config);
    return renderer.render(const_cast<Table&>(_table));
  }

  std::vector<Row> where(const std::string& column,
                         const std::string& value) const {
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

#endif  // DATABASE_HPP
