/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MySQLiteRepl.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 14:00:00 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 13:53:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MYSQLITE_REPL_HPP
#define MYSQLITE_REPL_HPP

// ============================================================================
//  MySQLite Interactive REPL
//  ─────────────────────────
//  A full Lexer → Parser → AST → Executor query engine that operates on
//  the Database / Table / Row / Column stack defined in Database.hpp and
//  Database_utils.hpp.
//
//  Architecture
//  ────────────
//    Input string
//      │
//      ▼
//    Lexer      ──  O(n) single-pass tokeniser
//      │
//      ▼
//    Parser     ──  recursive-descent, produces an AST node
//      │
//      ▼
//    Executor   ──  walks the AST, mutates / queries the table catalog
//      │
//      ▼
//    Renderer   ──  TableRenderer from Database.hpp renders results
//
//  Supported SQL-like statements
//  ─────────────────────────────
//    LOAD   '<file.csv>' AS <table>           — import CSV into catalog
//    TABLES                                   — list all loaded tables
//    DESCRIBE <table>                         — show columns, types, counts
//    SELECT [* | col,...] FROM <table>
//           [WHERE col op value]
//           [ORDER BY col [ASC|DESC]]
//           [LIMIT n]                         — query & render rows
//    INSERT INTO <table> (col,...) VALUES (v,...)  — add a row
//    UPDATE <table> SET col=val [WHERE ...]   — update matching rows
//    DELETE FROM <table> [WHERE ...]          — remove matching rows
//    ALTER  <table> ADD COLUMN <name> [type]  — add a column
//    ALTER  <table> DROP COLUMN <name>        — remove a column
//    ALTER  <table> RENAME COLUMN <old> TO <new>
//    ALTER  <table> MODIFY COLUMN <col> <type>
//    DROP   TABLE <table>                     — remove table from catalog
//    CREATE TABLE <name> (col type, ...)      — create empty table
//    COUNT  <table> [WHERE col op val]        — count rows
//    SUM    <col> FROM <table> [WHERE ...]    — aggregate
//    AVG    <col> FROM <table> [WHERE ...]
//    MIN    <col> FROM <table> [WHERE ...]
//    MAX    <col> FROM <table> [WHERE ...]
//    STATS  <col> FROM <table>                — full statistics
//    EXPORT <table> TO CSV '<path>'
//    EXPORT <table> TO HTML '<path>'
//    EXPORT <table> TO MARKDOWN
//    STYLE  <name>                            — ocean/matrix/fire/…
//    HELP                                     — show this summary
//    QUIT / EXIT                              — leave the REPL
//
//  All keywords are case-insensitive.  String literals use single quotes.
// ============================================================================

#include "Database.hpp"
#include "Database_utils.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <stdexcept>
#include <algorithm>
#include <cstdio>

// readline — C library, needs extern "C" linkage
extern "C" {
#include <readline/readline.h>
#include <readline/history.h>
}

#include <csignal>

// ── SIGINT handler for Ctrl+C ──────────────────────────────────────────
// Clears the current input line and redisplays the prompt instead of
// killing the process.
namespace MySQLite {
namespace _detail {
    static volatile sig_atomic_t g_sigint = 0;

    extern "C" {
        static void _sigintHandler(int) {
            g_sigint = 1;
            // Tell readline to abort the current line
            rl_done = 1;
        }
    }
} // namespace _detail
} // namespace MySQLite

namespace MySQLite {

// ════════════════════════════════════════════════════════════════════════
//  Token types produced by the Lexer
// ════════════════════════════════════════════════════════════════════════

namespace TK {
    enum Type {
        // Keywords
        SELECT, FROM, WHERE, ORDER, BY, ASC, DESC, LIMIT,
        INSERT, INTO, VALUES,
        UPDATE, SET,
        DELETE_KW,
        ALTER, ADD, DROP, RENAME, MODIFY, COLUMN, TO, TABLE,
        CREATE,
        LOAD, AS,
        TABLES, DESCRIBE,
        COUNT_KW, SUM_KW, AVG_KW, MIN_KW, MAX_KW, STATS_KW,
        EXPORT, CSV_KW, HTML_KW, MARKDOWN_KW,
        STYLE_KW, HELP, QUIT, EXIT,
        SHOW, DATABASES,
        AND_KW, OR_KW, NOT_KW,
        // Types
        T_STRING, T_INTEGER, T_DOUBLE, T_DATE, T_BOOLEAN,
        // Symbols
        STAR, COMMA, LPAREN, RPAREN, EQ, NEQ, LT, GT, LTE, GTE, DOT, SEMICOLON,
        // Literals
        STRING_LIT, NUMBER_LIT, IDENTIFIER,
        // End
        END_OF_INPUT,
        UNKNOWN
    };
}

struct Token {
    TK::Type    type;
    std::string value;
    size_t      pos;

    Token() : type(TK::END_OF_INPUT), pos(0) {}
    Token(TK::Type t, const std::string& v, size_t p) : type(t), value(v), pos(p) {}
};

// ════════════════════════════════════════════════════════════════════════
//  Lexer — single-pass O(n) tokeniser
// ════════════════════════════════════════════════════════════════════════

class Lexer {
public:
    explicit Lexer(const std::string& input) : _input(input), _pos(0) {
        _initKeywords();
    }

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        while (_pos < _input.size()) {
            _skipWhitespace();
            if (_pos >= _input.size()) break;

            size_t start = _pos;
            char c = _input[_pos];

            // Single-char symbols
            if (c == '*') { tokens.push_back(Token(TK::STAR, "*", start)); ++_pos; continue; }
            if (c == ',') { tokens.push_back(Token(TK::COMMA, ",", start)); ++_pos; continue; }
            if (c == '(') { tokens.push_back(Token(TK::LPAREN, "(", start)); ++_pos; continue; }
            if (c == ')') { tokens.push_back(Token(TK::RPAREN, ")", start)); ++_pos; continue; }
            if (c == '.') { tokens.push_back(Token(TK::DOT, ".", start)); ++_pos; continue; }
            if (c == ';') { tokens.push_back(Token(TK::SEMICOLON, ";", start)); ++_pos; continue; }

            // Two-char operators
            if (c == '!' && _peek(1) == '=') { tokens.push_back(Token(TK::NEQ, "!=", start)); _pos += 2; continue; }
            if (c == '<' && _peek(1) == '=') { tokens.push_back(Token(TK::LTE, "<=", start)); _pos += 2; continue; }
            if (c == '>' && _peek(1) == '=') { tokens.push_back(Token(TK::GTE, ">=", start)); _pos += 2; continue; }
            if (c == '<' && _peek(1) == '>') { tokens.push_back(Token(TK::NEQ, "<>", start)); _pos += 2; continue; }
            if (c == '<') { tokens.push_back(Token(TK::LT, "<", start)); ++_pos; continue; }
            if (c == '>') { tokens.push_back(Token(TK::GT, ">", start)); ++_pos; continue; }
            if (c == '=') { tokens.push_back(Token(TK::EQ, "=", start)); ++_pos; continue; }

            // String literal: 'text' or "text"
            if (c == '\'' || c == '"') {
                tokens.push_back(_readString());
                continue;
            }

            // Number literal (including negative) — or date literal (YYYY-MM-DD)
            if (std::isdigit(c) || (c == '-' && _pos + 1 < _input.size() && std::isdigit(_input[_pos + 1]))) {
                tokens.push_back(_readNumberOrDate());
                continue;
            }

            // Identifier / keyword
            if (std::isalpha(c) || c == '_') {
                tokens.push_back(_readIdentifier());
                continue;
            }

            // Unknown character — skip
            tokens.push_back(Token(TK::UNKNOWN, std::string(1, c), start));
            ++_pos;
        }
        tokens.push_back(Token(TK::END_OF_INPUT, "", _pos));
        return tokens;
    }

private:
    std::string _input;
    size_t _pos;
    std::map<std::string, TK::Type> _keywords;

    char _peek(size_t offset) const {
        return (_pos + offset < _input.size()) ? _input[_pos + offset] : '\0';
    }

    void _skipWhitespace() {
        while (_pos < _input.size() && std::isspace(_input[_pos])) ++_pos;
    }

    Token _readString() {
        size_t start = _pos;
        char quote = _input[_pos]; // ' or "
        ++_pos; // skip opening quote
        std::string val;
        while (_pos < _input.size() && _input[_pos] != quote) {
            if (_input[_pos] == '\\' && _pos + 1 < _input.size()) {
                ++_pos;
                val += _input[_pos];
            } else {
                val += _input[_pos];
            }
            ++_pos;
        }
        if (_pos < _input.size()) ++_pos; // skip closing quote
        return Token(TK::STRING_LIT, val, start);
    }

    // Read a number.  If it looks like a date (YYYY-MM-DD), read the
    // whole thing as a STRING_LIT so WHERE date=2022-03-29 works.
    Token _readNumberOrDate() {
        size_t start = _pos;
        std::string val;
        if (_input[_pos] == '-') { val += '-'; ++_pos; }
        while (_pos < _input.size() && (std::isdigit(_input[_pos]) || _input[_pos] == '.')) {
            val += _input[_pos];
            ++_pos;
        }
        // Check if this continues as a date: digits followed by '-' digit
        // Pattern: NNNN-NN-NN  (date literal)
        if (_pos < _input.size() && _input[_pos] == '-'
            && _pos + 1 < _input.size() && std::isdigit(_input[_pos + 1])) {
            // Looks like a date — keep reading segments separated by '-'
            while (_pos < _input.size() && _input[_pos] == '-'
                   && _pos + 1 < _input.size() && std::isdigit(_input[_pos + 1])) {
                val += _input[_pos]; ++_pos; // the '-'
                while (_pos < _input.size() && std::isdigit(_input[_pos])) {
                    val += _input[_pos]; ++_pos;
                }
            }
            return Token(TK::STRING_LIT, val, start);
        }
        return Token(TK::NUMBER_LIT, val, start);
    }

    Token _readIdentifier() {
        size_t start = _pos;
        std::string val;
        while (_pos < _input.size() && (std::isalnum(_input[_pos]) || _input[_pos] == '_')) {
            val += _input[_pos];
            ++_pos;
        }
        // Look up keyword (case-insensitive)
        std::string upper = _toUpper(val);
        std::map<std::string, TK::Type>::const_iterator it = _keywords.find(upper);
        if (it != _keywords.end())
            return Token(it->second, val, start);
        return Token(TK::IDENTIFIER, val, start);
    }

    static std::string _toUpper(const std::string& s) {
        std::string r = s;
        for (size_t i = 0; i < r.size(); ++i)
            r[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(r[i])));
        return r;
    }

    void _initKeywords() {
        _keywords["SELECT"]   = TK::SELECT;
        _keywords["FROM"]     = TK::FROM;
        _keywords["WHERE"]    = TK::WHERE;
        _keywords["ORDER"]    = TK::ORDER;
        _keywords["BY"]       = TK::BY;
        _keywords["ASC"]      = TK::ASC;
        _keywords["DESC"]     = TK::DESC;
        _keywords["LIMIT"]    = TK::LIMIT;
        _keywords["INSERT"]   = TK::INSERT;
        _keywords["INTO"]     = TK::INTO;
        _keywords["VALUES"]   = TK::VALUES;
        _keywords["UPDATE"]   = TK::UPDATE;
        _keywords["SET"]      = TK::SET;
        _keywords["DELETE"]   = TK::DELETE_KW;
        _keywords["ALTER"]    = TK::ALTER;
        _keywords["ADD"]      = TK::ADD;
        _keywords["DROP"]     = TK::DROP;
        _keywords["RENAME"]   = TK::RENAME;
        _keywords["MODIFY"]   = TK::MODIFY;
        _keywords["COLUMN"]   = TK::COLUMN;
        _keywords["TO"]       = TK::TO;
        _keywords["TABLE"]    = TK::TABLE;
        _keywords["CREATE"]   = TK::CREATE;
        _keywords["LOAD"]     = TK::LOAD;
        _keywords["AS"]       = TK::AS;
        _keywords["TABLES"]   = TK::TABLES;
        _keywords["DESCRIBE"] = TK::DESCRIBE;
        _keywords["COUNT"]    = TK::COUNT_KW;
        _keywords["SUM"]      = TK::SUM_KW;
        _keywords["AVG"]      = TK::AVG_KW;
        _keywords["MIN"]      = TK::MIN_KW;
        _keywords["MAX"]      = TK::MAX_KW;
        _keywords["STATS"]    = TK::STATS_KW;
        _keywords["EXPORT"]   = TK::EXPORT;
        _keywords["CSV"]      = TK::CSV_KW;
        _keywords["HTML"]     = TK::HTML_KW;
        _keywords["MARKDOWN"] = TK::MARKDOWN_KW;
        _keywords["STYLE"]    = TK::STYLE_KW;
        _keywords["SHOW"]     = TK::SHOW;
        _keywords["DATABASES"]= TK::DATABASES;
        _keywords["HELP"]     = TK::HELP;
        _keywords["QUIT"]     = TK::QUIT;
        _keywords["EXIT"]     = TK::EXIT;
        _keywords["AND"]      = TK::AND_KW;
        _keywords["OR"]       = TK::OR_KW;
        _keywords["NOT"]      = TK::NOT_KW;
        // Type keywords
        _keywords["STRING"]   = TK::T_STRING;
        _keywords["TEXT"]     = TK::T_STRING;
        _keywords["INTEGER"]  = TK::T_INTEGER;
        _keywords["INT"]      = TK::T_INTEGER;
        _keywords["DOUBLE"]   = TK::T_DOUBLE;
        _keywords["FLOAT"]    = TK::T_DOUBLE;
        _keywords["REAL"]     = TK::T_DOUBLE;
        _keywords["DATE"]     = TK::T_DATE;
        _keywords["BOOLEAN"]  = TK::T_BOOLEAN;
        _keywords["BOOL"]     = TK::T_BOOLEAN;
    }
};

// ════════════════════════════════════════════════════════════════════════
//  AST nodes produced by the Parser
// ════════════════════════════════════════════════════════════════════════

namespace AST {

    // ── WHERE condition ─────────────────────────────────────────────────

    struct Condition {
        std::string column;
        std::string op;       // =  !=  <  >  <=  >=
        std::string value;
        std::string logic;    // ""  AND  OR  (for chaining)

        Condition() {}
    };

    // ── Statement types ─────────────────────────────────────────────────

    enum StmtType {
        STMT_SELECT, STMT_INSERT, STMT_UPDATE, STMT_DELETE,
        STMT_CREATE, STMT_DROP_TABLE, STMT_ALTER,
        STMT_LOAD, STMT_TABLES, STMT_DESCRIBE, STMT_SHOW_DATABASES,
        STMT_AGGREGATE, STMT_STATS,
        STMT_EXPORT, STMT_STYLE, STMT_HELP, STMT_QUIT,
        STMT_UNKNOWN
    };

    enum AlterAction { ALT_ADD_COL, ALT_DROP_COL, ALT_RENAME_COL, ALT_MODIFY_COL };
    enum AggFunc     { AGG_COUNT, AGG_SUM, AGG_AVG, AGG_MIN, AGG_MAX };
    enum ExportFmt   { EXP_CSV, EXP_HTML, EXP_MARKDOWN };

    struct Statement {
        StmtType type;

        // SELECT / aggregate
        std::vector<std::string> columns;     // * → empty
        std::string              tableName;
        std::vector<Condition>   conditions;
        std::string              orderColumn;
        bool                     orderAsc;
        size_t                   limitN;

        // INSERT
        std::vector<std::string> insertCols;
        std::vector<std::string> insertVals;

        // UPDATE
        std::map<std::string, std::string> setClauses;

        // ALTER
        AlterAction  alterAction;
        std::string  alterCol;
        std::string  alterNewName;       // for RENAME
        ColumnType::Type alterColType;   // for ADD / MODIFY

        // AGGREGATE
        AggFunc      aggFunc;
        std::string  aggColumn;

        // EXPORT
        ExportFmt    exportFmt;
        std::string  exportPath;

        // STYLE
        std::string  styleName;

        // LOAD
        std::string  loadPath;
        std::string  loadAlias;

        // CREATE
        std::vector<std::string>       createCols;
        std::vector<ColumnType::Type>  createTypes;

        Statement()
            : type(STMT_UNKNOWN), orderAsc(true), limitN(0),
              alterAction(ALT_ADD_COL), alterColType(ColumnType::STRING),
              aggFunc(AGG_COUNT), exportFmt(EXP_CSV) {}
    };
}

// ════════════════════════════════════════════════════════════════════════
//  Parser — recursive descent, produces AST::Statement
// ════════════════════════════════════════════════════════════════════════

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : _tokens(tokens), _pos(0) {}

    AST::Statement parse() {
        if (_cur().type == TK::END_OF_INPUT)
            throw std::runtime_error("Empty statement.");

        switch (_cur().type) {
            case TK::SELECT:    return _parseSelect();
            case TK::INSERT:    return _parseInsert();
            case TK::UPDATE:    return _parseUpdate();
            case TK::DELETE_KW: return _parseDelete();
            case TK::CREATE:    return _parseCreate();
            case TK::DROP:      return _parseDrop();
            case TK::ALTER:     return _parseAlter();
            case TK::LOAD:      return _parseLoad();
            case TK::TABLES:    return _parseTables();
            case TK::SHOW:      return _parseShow();
            case TK::DESCRIBE:  return _parseDescribe();
            case TK::COUNT_KW:  return _parseAggregate(AST::AGG_COUNT);
            case TK::SUM_KW:    return _parseAggregate(AST::AGG_SUM);
            case TK::AVG_KW:    return _parseAggregate(AST::AGG_AVG);
            case TK::MIN_KW:    return _parseAggregate(AST::AGG_MIN);
            case TK::MAX_KW:    return _parseAggregate(AST::AGG_MAX);
            case TK::STATS_KW:  return _parseStats();
            case TK::EXPORT:    return _parseExport();
            case TK::STYLE_KW:  return _parseStyle();
            case TK::HELP:      { AST::Statement s; s.type = AST::STMT_HELP; return s; }
            case TK::QUIT:
            case TK::EXIT:      { AST::Statement s; s.type = AST::STMT_QUIT; return s; }
            default:
                throw std::runtime_error(std::string("Unexpected token: '") + _cur().value + "'");
        }
    }

private:
    std::vector<Token> _tokens;
    size_t _pos;

    const Token& _cur() const { return _tokens[_pos]; }
    const Token& _advance() { return _tokens[_pos++]; }

    bool _match(TK::Type t) {
        if (_cur().type == t) { ++_pos; return true; }
        return false;
    }

    void _expect(TK::Type t, const std::string& ctx = "") {
        if (_cur().type != t) {
            std::ostringstream oss;
            oss << "Expected ";
            if (!ctx.empty()) oss << ctx;
            else oss << "token type " << t;
            oss << " but got '" << _cur().value << "'";
            throw std::runtime_error(oss.str());
        }
        ++_pos;
    }

    // Read any identifier or keyword as a plain name
    std::string _readName() {
        if (_cur().type == TK::IDENTIFIER || (_cur().type >= TK::SELECT && _cur().type <= TK::T_BOOLEAN)) {
            return _advance().value;
        }
        throw std::runtime_error(std::string("Expected name, got '") + _cur().value + "'");
    }

    // Read a literal value (string, number, or bare identifier)
    std::string _readValue() {
        if (_cur().type == TK::STRING_LIT || _cur().type == TK::NUMBER_LIT)
            return _advance().value;
        if (_cur().type == TK::IDENTIFIER)
            return _advance().value;
        // Also allow keyword tokens as values (e.g. "true", "false", type names used as values)
        if (_cur().type >= TK::SELECT && _cur().type <= TK::T_BOOLEAN)
            return _advance().value;
        throw std::runtime_error(std::string("Expected value, got '") + _cur().value + "'");
    }

    ColumnType::Type _readType() {
        switch (_cur().type) {
            case TK::T_STRING:  ++_pos; return ColumnType::STRING;
            case TK::T_INTEGER: ++_pos; return ColumnType::INTEGER;
            case TK::T_DOUBLE:  ++_pos; return ColumnType::DOUBLE;
            case TK::T_DATE:    ++_pos; return ColumnType::DATE;
            case TK::T_BOOLEAN: ++_pos; return ColumnType::BOOLEAN;
            default: return ColumnType::STRING; // implicit STRING if omitted
        }
    }

    // ── WHERE clause ────────────────────────────────────────────────────

    std::vector<AST::Condition> _parseWhere() {
        std::vector<AST::Condition> conds;
        if (!_match(TK::WHERE)) return conds;

        while (true) {
            AST::Condition c;
            c.column = _readName();
            // operator
            if      (_match(TK::EQ))  c.op = "=";
            else if (_match(TK::NEQ)) c.op = "!=";
            else if (_match(TK::LT))  c.op = "<";
            else if (_match(TK::GT))  c.op = ">";
            else if (_match(TK::LTE)) c.op = "<=";
            else if (_match(TK::GTE)) c.op = ">=";
            else throw std::runtime_error("Expected comparison operator after column name in WHERE");

            c.value = _readValue();
            conds.push_back(c);

            // AND / OR chaining
            if (_cur().type == TK::AND_KW) {
                conds.back().logic = "AND";
                ++_pos;
            } else if (_cur().type == TK::OR_KW) {
                conds.back().logic = "OR";
                ++_pos;
            } else {
                break;
            }
        }
        return conds;
    }

    // ── ORDER BY ────────────────────────────────────────────────────────

    void _parseOrderBy(AST::Statement& s) {
        if (!_match(TK::ORDER)) return;
        _expect(TK::BY, "BY");
        s.orderColumn = _readName();
        s.orderAsc = true;
        if (_match(TK::DESC)) s.orderAsc = false;
        else _match(TK::ASC);
    }

    // ── LIMIT ───────────────────────────────────────────────────────────

    void _parseLimit(AST::Statement& s) {
        if (!_match(TK::LIMIT)) return;
        if (_cur().type != TK::NUMBER_LIT)
            throw std::runtime_error("Expected number after LIMIT");
        s.limitN = static_cast<size_t>(std::atoi(_advance().value.c_str()));
    }

    // ── SELECT ──────────────────────────────────────────────────────────

    AST::Statement _parseSelect() {
        AST::Statement s;
        s.type = AST::STMT_SELECT;
        _advance(); // consume SELECT

        // Column list or *
        if (_match(TK::STAR)) {
            // columns stays empty → means all
        } else {
            s.columns.push_back(_readName());
            while (_match(TK::COMMA)) {
                s.columns.push_back(_readName());
            }
        }

        _expect(TK::FROM, "FROM");
        s.tableName = _readName();
        s.conditions = _parseWhere();
        _parseOrderBy(s);
        _parseLimit(s);
        return s;
    }

    // ── INSERT ──────────────────────────────────────────────────────────

    AST::Statement _parseInsert() {
        AST::Statement s;
        s.type = AST::STMT_INSERT;
        _advance(); // INSERT
        _expect(TK::INTO, "INTO");
        s.tableName = _readName();

        _expect(TK::LPAREN, "(");
        s.insertCols.push_back(_readName());
        while (_match(TK::COMMA)) s.insertCols.push_back(_readName());
        _expect(TK::RPAREN, ")");

        _expect(TK::VALUES, "VALUES");
        _expect(TK::LPAREN, "(");
        s.insertVals.push_back(_readValue());
        while (_match(TK::COMMA)) s.insertVals.push_back(_readValue());
        _expect(TK::RPAREN, ")");

        return s;
    }

    // ── UPDATE ──────────────────────────────────────────────────────────

    AST::Statement _parseUpdate() {
        AST::Statement s;
        s.type = AST::STMT_UPDATE;
        _advance(); // UPDATE
        s.tableName = _readName();
        _expect(TK::SET, "SET");

        // col = val [, col = val ...]
        while (true) {
            std::string col = _readName();
            _expect(TK::EQ, "=");
            std::string val = _readValue();
            s.setClauses[col] = val;
            if (!_match(TK::COMMA)) break;
        }

        s.conditions = _parseWhere();
        return s;
    }

    // ── DELETE ──────────────────────────────────────────────────────────

    AST::Statement _parseDelete() {
        AST::Statement s;
        s.type = AST::STMT_DELETE;
        _advance(); // DELETE
        _expect(TK::FROM, "FROM");
        s.tableName = _readName();
        s.conditions = _parseWhere();
        return s;
    }

    // ── CREATE TABLE ────────────────────────────────────────────────────

    AST::Statement _parseCreate() {
        AST::Statement s;
        s.type = AST::STMT_CREATE;
        _advance(); // CREATE
        _expect(TK::TABLE, "TABLE");
        s.tableName = _readName();

        _expect(TK::LPAREN, "(");
        while (true) {
            s.createCols.push_back(_readName());
            s.createTypes.push_back(_readType());
            if (!_match(TK::COMMA)) break;
        }
        _expect(TK::RPAREN, ")");
        return s;
    }

    // ── DROP TABLE ──────────────────────────────────────────────────────

    AST::Statement _parseDrop() {
        AST::Statement s;
        s.type = AST::STMT_DROP_TABLE;
        _advance(); // DROP
        _expect(TK::TABLE, "TABLE");
        s.tableName = _readName();
        return s;
    }

    // ── ALTER TABLE ─────────────────────────────────────────────────────

    AST::Statement _parseAlter() {
        AST::Statement s;
        s.type = AST::STMT_ALTER;
        _advance(); // ALTER
        // TABLE is optional  ("ALTER employees" works too)
        _match(TK::TABLE);
        s.tableName = _readName();

        if (_match(TK::ADD)) {
            _match(TK::COLUMN); // optional
            s.alterAction = AST::ALT_ADD_COL;
            s.alterCol = _readName();
            s.alterColType = _readType();
        } else if (_cur().type == TK::DROP) {
            ++_pos;
            _match(TK::COLUMN);
            s.alterAction = AST::ALT_DROP_COL;
            s.alterCol = _readName();
        } else if (_match(TK::RENAME)) {
            _match(TK::COLUMN);
            s.alterAction = AST::ALT_RENAME_COL;
            s.alterCol = _readName();
            _expect(TK::TO, "TO");
            s.alterNewName = _readName();
        } else if (_match(TK::MODIFY)) {
            _match(TK::COLUMN);
            s.alterAction = AST::ALT_MODIFY_COL;
            s.alterCol = _readName();
            s.alterColType = _readType();
        } else {
            throw std::runtime_error("ALTER: expected ADD, DROP, RENAME, or MODIFY");
        }
        return s;
    }

    // ── LOAD ────────────────────────────────────────────────────────────

    AST::Statement _parseLoad() {
        AST::Statement s;
        s.type = AST::STMT_LOAD;
        _advance(); // LOAD
        if (_cur().type != TK::STRING_LIT)
            throw std::runtime_error("LOAD expects a file path in quotes, e.g. LOAD 'file.csv' AS name");
        s.loadPath = _advance().value;
        if (_match(TK::AS))
            s.loadAlias = _readName();
        else {
            // derive name from filename: strip path and extension
            std::string nm = s.loadPath;
            size_t sl = nm.rfind('/');
            if (sl != std::string::npos) nm = nm.substr(sl + 1);
            size_t dt = nm.rfind('.');
            if (dt != std::string::npos) nm = nm.substr(0, dt);
            s.loadAlias = nm;
        }
        return s;
    }

    // ── TABLES ──────────────────────────────────────────────────────────

    AST::Statement _parseTables() {
        AST::Statement s;
        s.type = AST::STMT_TABLES;
        _advance();
        return s;
    }

    // ── SHOW (TABLES | DATABASES | TABLE <name>) ────────────────────────

    AST::Statement _parseShow() {
        AST::Statement s;
        _advance(); // consume SHOW
        if (_match(TK::TABLES)) {
            s.type = AST::STMT_TABLES;
        } else if (_match(TK::DATABASES)) {
            s.type = AST::STMT_SHOW_DATABASES;
        } else if (_cur().type == TK::TABLE) {
            ++_pos; // consume TABLE
            s.type = AST::STMT_DESCRIBE;
            s.tableName = _readName();
        } else {
            throw std::runtime_error("SHOW expects TABLES, DATABASES, or TABLE <name>");
        }
        return s;
    }

    // ── DESCRIBE ────────────────────────────────────────────────────────

    AST::Statement _parseDescribe() {
        AST::Statement s;
        s.type = AST::STMT_DESCRIBE;
        _advance();
        s.tableName = _readName();
        return s;
    }

    // ── Aggregates: COUNT / SUM / AVG / MIN / MAX ───────────────────────

    AST::Statement _parseAggregate(AST::AggFunc fn) {
        AST::Statement s;
        s.type = AST::STMT_AGGREGATE;
        s.aggFunc = fn;
        _advance(); // keyword

        if (fn == AST::AGG_COUNT) {
            // COUNT <table> [WHERE ...]  or  COUNT * FROM <table> [WHERE ...]
            if (_match(TK::STAR)) {
                _expect(TK::FROM, "FROM");
                s.tableName = _readName();
            } else if (_cur().type == TK::IDENTIFIER) {
                // Could be "COUNT employees" shorthand or "COUNT col FROM table"
                std::string first = _readName();
                if (_match(TK::FROM)) {
                    s.tableName = _readName();
                    s.aggColumn = first;
                } else {
                    s.tableName = first;
                }
            }
        } else {
            // SUM/AVG/MIN/MAX <col> FROM <table>
            s.aggColumn = _readName();
            _expect(TK::FROM, "FROM");
            s.tableName = _readName();
        }

        s.conditions = _parseWhere();
        return s;
    }

    // ── STATS ───────────────────────────────────────────────────────────

    AST::Statement _parseStats() {
        AST::Statement s;
        s.type = AST::STMT_STATS;
        _advance();
        s.aggColumn = _readName();
        _expect(TK::FROM, "FROM");
        s.tableName = _readName();
        return s;
    }

    // ── EXPORT ──────────────────────────────────────────────────────────

    AST::Statement _parseExport() {
        AST::Statement s;
        s.type = AST::STMT_EXPORT;
        _advance(); // EXPORT
        s.tableName = _readName();
        _expect(TK::TO, "TO");

        if      (_match(TK::CSV_KW))      s.exportFmt = AST::EXP_CSV;
        else if (_match(TK::HTML_KW))      s.exportFmt = AST::EXP_HTML;
        else if (_match(TK::MARKDOWN_KW))  s.exportFmt = AST::EXP_MARKDOWN;
        else throw std::runtime_error("EXPORT: expected CSV, HTML, or MARKDOWN after TO");

        if (s.exportFmt != AST::EXP_MARKDOWN) {
            if (_cur().type != TK::STRING_LIT)
                throw std::runtime_error("EXPORT to CSV/HTML requires a path in quotes");
            s.exportPath = _advance().value;
        }
        return s;
    }

    // ── STYLE ───────────────────────────────────────────────────────────

    AST::Statement _parseStyle() {
        AST::Statement s;
        s.type = AST::STMT_STYLE;
        _advance();
        s.styleName = _readName();
        return s;
    }
};

// ════════════════════════════════════════════════════════════════════════
//  Executor — walks AST, mutates/queries the table catalog
// ════════════════════════════════════════════════════════════════════════

class Executor {
public:
    Executor() : _styleName("ocean") {}

    // Catalog access — lets the REPL pre-load tables
    void addTable(const std::string& name, const Database& db) {
        _catalog[name] = db;
    }

    std::string execute(const AST::Statement& stmt) {
        switch (stmt.type) {
            case AST::STMT_SELECT:     return _execSelect(stmt);
            case AST::STMT_INSERT:     return _execInsert(stmt);
            case AST::STMT_UPDATE:     return _execUpdate(stmt);
            case AST::STMT_DELETE:     return _execDelete(stmt);
            case AST::STMT_CREATE:     return _execCreate(stmt);
            case AST::STMT_DROP_TABLE: return _execDrop(stmt);
            case AST::STMT_ALTER:      return _execAlter(stmt);
            case AST::STMT_LOAD:       return _execLoad(stmt);
            case AST::STMT_TABLES:     return _execTables();
            case AST::STMT_SHOW_DATABASES: return _execShowDatabases();
            case AST::STMT_DESCRIBE:   return _execDescribe(stmt);
            case AST::STMT_AGGREGATE:  return _execAggregate(stmt);
            case AST::STMT_STATS:      return _execStats(stmt);
            case AST::STMT_EXPORT:     return _execExport(stmt);
            case AST::STMT_STYLE:      _styleName = _toLower(stmt.styleName); return _info("Style set to '" + _styleName + "'.");
            case AST::STMT_HELP:       return _helpText();
            case AST::STMT_QUIT:       return "";
            default: return _err("Unknown statement.");
        }
    }

private:
    std::map<std::string, Database> _catalog;   // name → Database
    std::string                     _styleName;

    // ── Catalog lookup ──────────────────────────────────────────────────

    Database& _getTable(const std::string& name) {
        std::map<std::string, Database>::iterator it = _catalog.find(name);
        if (it == _catalog.end())
            throw std::runtime_error(std::string("Table '") + name + "' not found. Use TABLES to list loaded tables.");
        return it->second;
    }

    // ── Style helper ────────────────────────────────────────────────────

    RenderConfig _getStyleConfig() const {
        if (_styleName == "matrix")    return StylePresets::matrix();
        if (_styleName == "fire")      return StylePresets::fire();
        if (_styleName == "corporate") return StylePresets::corporate();
        if (_styleName == "newspaper") return StylePresets::newspaper();
        if (_styleName == "elegant")   return StylePresets::elegant();
        if (_styleName == "minimal")   return RenderConfig::minimal();
        return StylePresets::ocean(); // default
    }

    // ── WHERE evaluation ────────────────────────────────────────────────

    static bool _evalCondition(const Row& row, const AST::Condition& c) {
        std::string val = row.getValue(c.column);
        // Try numeric comparison first
        char *endA = NULL, *endB = NULL;
        double na = std::strtod(val.c_str(), &endA);
        double nb = std::strtod(c.value.c_str(), &endB);
        bool numeric = (endA != val.c_str() && (endA == NULL || *endA == '\0'))
                    && (endB != c.value.c_str() && (endB == NULL || *endB == '\0'));

        if (c.op == "=")  return numeric ? (na == nb) : (val == c.value);
        if (c.op == "!=") return numeric ? (na != nb) : (val != c.value);
        if (c.op == "<")  return numeric ? (na <  nb) : (val <  c.value);
        if (c.op == ">")  return numeric ? (na >  nb) : (val >  c.value);
        if (c.op == "<=") return numeric ? (na <= nb) : (val <= c.value);
        if (c.op == ">=") return numeric ? (na >= nb) : (val >= c.value);
        return false;
    }

    static bool _matchRow(const Row& row, const std::vector<AST::Condition>& conds) {
        if (conds.empty()) return true;
        bool result = _evalCondition(row, conds[0]);
        for (size_t i = 1; i < conds.size(); ++i) {
            bool next = _evalCondition(row, conds[i]);
            if (conds[i - 1].logic == "OR")
                result = result || next;
            else // AND (default)
                result = result && next;
        }
        return result;
    }

    // ── Formatting helpers ──────────────────────────────────────────────

    static std::string _toLower(const std::string& s) {
        std::string r = s;
        for (size_t i = 0; i < r.size(); ++i)
            r[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(r[i])));
        return r;
    }

    static std::string _err(const std::string& msg) {
        return std::string("\033[91m✗ ") + msg + "\033[0m";
    }

    static std::string _info(const std::string& msg) {
        return std::string("\033[92m✓ ") + msg + "\033[0m";
    }

    static std::string _typeToStr(ColumnType::Type t) {
        switch (t) {
            case ColumnType::INTEGER: return "INTEGER";
            case ColumnType::DOUBLE:  return "DOUBLE";
            case ColumnType::DATE:    return "DATE";
            case ColumnType::BOOLEAN: return "BOOLEAN";
            default:                  return "STRING";
        }
    }

    // ── Render a Table with current style ───────────────────────────────

    std::string _renderTable(Table& tbl, const std::string& footer = "") const {
        RenderConfig cfg = _getStyleConfig();
        cfg.autoIncrementId = false;
        if (!footer.empty()) {
            cfg.showFooter = true;
            cfg.footerText = footer;
            cfg.footerStyle.foreground = Style::Color::BrightYellow();
            cfg.footerStyle.bold = true;
        }
        TableRenderer renderer(cfg);
        return renderer.render(tbl);
    }

    // ── Build a result Table from filtered rows + column subset ─────────

    Table _buildResult(const Table& source,
                       const std::vector<std::string>& cols,
                       const std::vector<Row>& rows) const
    {
        Table result;
        const std::vector<Column>& srcCols = source.columns();

        // Which columns to include
        if (cols.empty()) {
            // all columns
            for (size_t i = 0; i < srcCols.size(); ++i)
                result.addColumn(srcCols[i]);
        } else {
            for (size_t i = 0; i < cols.size(); ++i) {
                bool found = false;
                for (size_t j = 0; j < srcCols.size(); ++j) {
                    if (srcCols[j].name() == cols[i]) {
                        result.addColumn(srcCols[j]);
                        found = true;
                        break;
                    }
                }
                if (!found)
                    result.addColumn(Column(cols[i]));
            }
        }

        for (size_t i = 0; i < rows.size(); ++i)
            result.addRow(rows[i]);

        return result;
    }

    // ════════════════════════════════════════════════════════════════════
    //  Statement executors
    // ════════════════════════════════════════════════════════════════════

    // ── SELECT ──────────────────────────────────────────────────────────

    std::string _execSelect(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        const Table& tbl = db.table();

        // Filter rows
        std::vector<Row> rows;
        const std::vector<Row>& allRows = tbl.rows();
        for (size_t i = 0; i < allRows.size(); ++i) {
            if (_matchRow(allRows[i], s.conditions))
                rows.push_back(allRows[i]);
        }

        // Sort
        if (!s.orderColumn.empty()) {
            std::sort(rows.begin(), rows.end(),
                      Transform::RowComparator(s.orderColumn, s.orderAsc));
        }

        // Limit
        if (s.limitN > 0 && rows.size() > s.limitN)
            rows.resize(s.limitN);

        Table result = _buildResult(tbl, s.columns, rows);

        std::ostringstream footer;
        footer << rows.size() << " row" << (rows.size() != 1 ? "s" : "") << " selected";
        return _renderTable(result, footer.str());
    }

    // ── INSERT ──────────────────────────────────────────────────────────

    std::string _execInsert(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        if (s.insertCols.size() != s.insertVals.size())
            return _err("Column count doesn't match value count.");

        std::map<std::string, std::string> rowData;
        for (size_t i = 0; i < s.insertCols.size(); ++i)
            rowData[s.insertCols[i]] = s.insertVals[i];

        db.addRow(rowData);
        return _info("1 row inserted into '" + s.tableName + "'.");
    }

    // ── UPDATE ──────────────────────────────────────────────────────────

    std::string _execUpdate(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        std::vector<Row>& rows = db.table().rows();
        size_t affected = 0;

        for (size_t i = 0; i < rows.size(); ++i) {
            if (_matchRow(rows[i], s.conditions)) {
                for (std::map<std::string, std::string>::const_iterator it = s.setClauses.begin();
                     it != s.setClauses.end(); ++it) {
                    rows[i].setValue(it->first, it->second);
                }
                ++affected;
            }
        }
        std::ostringstream oss;
        oss << affected << " row" << (affected != 1 ? "s" : "") << " updated.";
        return _info(oss.str());
    }

    // ── DELETE ──────────────────────────────────────────────────────────

    std::string _execDelete(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        std::vector<Row>& rows = db.table().rows();
        size_t before = rows.size();

        std::vector<Row> kept;
        for (size_t i = 0; i < rows.size(); ++i) {
            if (!_matchRow(rows[i], s.conditions))
                kept.push_back(rows[i]);
        }
        rows = kept;

        size_t deleted = before - rows.size();
        std::ostringstream oss;
        oss << deleted << " row" << (deleted != 1 ? "s" : "") << " deleted.";
        return _info(oss.str());
    }

    // ── CREATE TABLE ────────────────────────────────────────────────────

    std::string _execCreate(const AST::Statement& s) {
        if (_catalog.find(s.tableName) != _catalog.end())
            return _err("Table '" + s.tableName + "' already exists.");

        Database db;
        for (size_t i = 0; i < s.createCols.size(); ++i)
            db.addColumn(s.createCols[i], s.createTypes[i]);

        _catalog[s.tableName] = db;
        std::ostringstream oss;
        oss << "Table '" << s.tableName << "' created with " << s.createCols.size() << " column(s).";
        return _info(oss.str());
    }

    // ── DROP TABLE ──────────────────────────────────────────────────────

    std::string _execDrop(const AST::Statement& s) {
        if (_catalog.find(s.tableName) == _catalog.end())
            return _err("Table '" + s.tableName + "' not found.");
        _catalog.erase(_catalog.find(s.tableName));
        return _info("Table '" + s.tableName + "' dropped.");
    }

    // ── ALTER TABLE ─────────────────────────────────────────────────────

    std::string _execAlter(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        Table& tbl = db.table();
        std::vector<Column>& cols = tbl.columns();

        switch (s.alterAction) {
            case AST::ALT_ADD_COL: {
                // Check duplicate
                for (size_t i = 0; i < cols.size(); ++i) {
                    if (cols[i].name() == s.alterCol)
                        return _err("Column '" + s.alterCol + "' already exists.");
                }
                tbl.addColumn(Column(s.alterCol, s.alterColType));
                return _info("Column '" + s.alterCol + "' added to '" + s.tableName + "'.");
            }
            case AST::ALT_DROP_COL: {
                bool found = false;
                for (std::vector<Column>::iterator it = cols.begin(); it != cols.end(); ++it) {
                    if (it->name() == s.alterCol) {
                        cols.erase(it);
                        found = true;
                        break;
                    }
                }
                if (!found) return _err("Column '" + s.alterCol + "' not found.");
                return _info("Column '" + s.alterCol + "' dropped from '" + s.tableName + "'.");
            }
            case AST::ALT_RENAME_COL: {
                for (size_t i = 0; i < cols.size(); ++i) {
                    if (cols[i].name() == s.alterCol) {
                        cols[i] = Column(s.alterNewName, cols[i].type(), cols[i].alignment());
                        // Rename in all rows too
                        std::vector<Row>& rows = tbl.rows();
                        for (size_t r = 0; r < rows.size(); ++r) {
                            std::string val = rows[r].getValue(s.alterCol);
                            rows[r].setValue(s.alterNewName, val);
                        }
                        return _info("Column '" + s.alterCol + "' renamed to '" + s.alterNewName + "'.");
                    }
                }
                return _err("Column '" + s.alterCol + "' not found.");
            }
            case AST::ALT_MODIFY_COL: {
                for (size_t i = 0; i < cols.size(); ++i) {
                    if (cols[i].name() == s.alterCol) {
                        cols[i] = Column(s.alterCol, s.alterColType, cols[i].alignment());
                        return _info("Column '" + s.alterCol + "' type changed to " + _typeToStr(s.alterColType) + ".");
                    }
                }
                return _err("Column '" + s.alterCol + "' not found.");
            }
        }
        return _err("Unknown alter action.");
    }

    // ── LOAD ────────────────────────────────────────────────────────────

    std::string _execLoad(const AST::Statement& s) {
        Database db;
        RenderConfig cfg;
        cfg.autoIncrementId = true;
        db.loadFromCsv(s.loadPath, true, cfg);
        _catalog[s.loadAlias] = db;

        std::ostringstream oss;
        oss << "Loaded '" << s.loadPath << "' as '" << s.loadAlias
            << "' (" << db.count() << " rows, "
            << db.table().columnCount() << " columns).";
        return _info(oss.str());
    }

    // ── TABLES / SHOW TABLES ────────────────────────────────────────────

    std::string _execTables() {
        if (_catalog.empty())
            return _info("No tables loaded. Use LOAD 'file.csv' AS name;");

        Database listing;
        listing.addColumn("Tables_in_mysqlite", ColumnType::STRING, Alignment::LEFT);
        listing.addColumn("Columns", ColumnType::STRING, Alignment::LEFT);

        for (std::map<std::string, Database>::const_iterator it = _catalog.begin();
             it != _catalog.end(); ++it) {
            std::map<std::string, std::string> row;
            row["Tables_in_mysqlite"] = it->first;

            // Build comma-separated column names
            const std::vector<Column>& cols = it->second.table().columns();
            std::string colNames;
            for (size_t c = 0; c < cols.size(); ++c) {
                if (c > 0) colNames += ", ";
                colNames += cols[c].name();
            }
            row["Columns"] = colNames;
            listing.addRow(row);
        }

        Table tbl = listing.table();
        std::ostringstream footer;
        footer << _catalog.size() << " row" << (_catalog.size() != 1 ? "s" : "") << " in set";
        return _renderTable(tbl, footer.str());
    }

    // ── SHOW DATABASES ──────────────────────────────────────────────────

    std::string _execShowDatabases() {
        if (_catalog.empty())
            return _info("No databases. Use LOAD 'file.csv' AS name;");

        Database listing;
        listing.addColumn("Database", ColumnType::STRING, Alignment::LEFT);

        std::map<std::string, std::string> row;
        row["Database"] = "mysqlite";
        listing.addRow(row);

        Table dtbl = listing.table();
        return _renderTable(dtbl, "1 row in set");
    }

    // ── DESCRIBE ────────────────────────────────────────────────────────

    std::string _execDescribe(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        const Table& tbl = db.table();
        const std::vector<Column>& cols = tbl.columns();

        Database desc;
        desc.addColumn("Column", ColumnType::STRING, Alignment::LEFT);
        desc.addColumn("Type", ColumnType::STRING, Alignment::LEFT);
        desc.addColumn("Alignment", ColumnType::STRING, Alignment::CENTER);
        desc.addColumn("Non-Empty", ColumnType::INTEGER, Alignment::RIGHT);
        desc.addColumn("Sample", ColumnType::STRING, Alignment::LEFT);

        for (size_t i = 0; i < cols.size(); ++i) {
            std::map<std::string, std::string> row;
            row["Column"] = cols[i].name();
            row["Type"] = _typeToStr(cols[i].type());
            row["Alignment"] = (cols[i].alignment() == Alignment::LEFT) ? "LEFT" :
                               (cols[i].alignment() == Alignment::RIGHT) ? "RIGHT" : "CENTER";
            // Count non-empty
            size_t nonEmpty = 0;
            std::string sample;
            const std::vector<Row>& rows = tbl.rows();
            for (size_t r = 0; r < rows.size(); ++r) {
                std::string v = rows[r].getValue(cols[i].name());
                if (!v.empty()) {
                    ++nonEmpty;
                    if (sample.empty()) sample = v;
                }
            }
            { std::ostringstream o; o << nonEmpty; row["Non-Empty"] = o.str(); }
            if (sample.size() > 30) sample = sample.substr(0, 27) + "...";
            row["Sample"] = sample;
            desc.addRow(row);
        }

        Table dtbl = desc.table();
        std::ostringstream footer;
        footer << "'" << s.tableName << "': " << cols.size() << " columns, " << tbl.rowCount() << " rows";
        return _renderTable(dtbl, footer.str());
    }

    // ── AGGREGATE ───────────────────────────────────────────────────────

    std::string _execAggregate(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        const Table& tbl = db.table();

        // Apply WHERE filter if present
        std::vector<Row> rows;
        const std::vector<Row>& allRows = tbl.rows();
        for (size_t i = 0; i < allRows.size(); ++i) {
            if (_matchRow(allRows[i], s.conditions))
                rows.push_back(allRows[i]);
        }

        // Build temp table for the aggregate
        Table filtered;
        for (size_t i = 0; i < tbl.columns().size(); ++i)
            filtered.addColumn(tbl.columns()[i]);
        for (size_t i = 0; i < rows.size(); ++i)
            filtered.addRow(rows[i]);

        double result = 0;
        std::string funcName;

        switch (s.aggFunc) {
            case AST::AGG_COUNT:
                funcName = "COUNT";
                result = static_cast<double>(filtered.rowCount());
                break;
            case AST::AGG_SUM:
                funcName = "SUM";
                result = Query::sum(filtered, s.aggColumn);
                break;
            case AST::AGG_AVG:
                funcName = "AVG";
                result = Query::average(filtered, s.aggColumn);
                break;
            case AST::AGG_MIN:
                funcName = "MIN";
                result = Query::min(filtered, s.aggColumn);
                break;
            case AST::AGG_MAX:
                funcName = "MAX";
                result = Query::max(filtered, s.aggColumn);
                break;
        }

        // Build a little result table
        Database resDb;
        resDb.addColumn("Function", ColumnType::STRING, Alignment::LEFT);
        resDb.addColumn("Column", ColumnType::STRING, Alignment::LEFT);
        resDb.addColumn("Result", ColumnType::DOUBLE, Alignment::RIGHT);

        std::map<std::string, std::string> row;
        row["Function"] = funcName;
        row["Column"] = s.aggColumn.empty() ? "*" : s.aggColumn;
        { std::ostringstream o; o << result; row["Result"] = o.str(); }
        resDb.addRow(row);

        Table rtbl = resDb.table();
        return _renderTable(rtbl);
    }

    // ── STATS ───────────────────────────────────────────────────────────

    std::string _execStats(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);
        Statistics::ColumnStats stats = Statistics::analyze(db.table(), s.aggColumn);
        Database statsDb = Statistics::createStatsTable(stats, s.aggColumn);

        RenderConfig cfg = StylePresets::corporate();
        cfg.autoIncrementId = false;
        cfg.showFooter = true;
        cfg.footerText = "Statistics for '" + s.aggColumn + "' in '" + s.tableName + "'";
        cfg.footerStyle.foreground = Style::Color::BrightCyan();
        cfg.footerStyle.bold = true;

        return statsDb.render(cfg);
    }

    // ── EXPORT ──────────────────────────────────────────────────────────

    std::string _execExport(const AST::Statement& s) {
        Database& db = _getTable(s.tableName);

        switch (s.exportFmt) {
            case AST::EXP_CSV:
                Export::toCsv(db.table(), s.exportPath);
                return _info("Table '" + s.tableName + "' exported to CSV: " + s.exportPath);
            case AST::EXP_HTML: {
                std::string html = Export::toHtml(db.table(), s.tableName);
                std::ofstream f(s.exportPath.c_str());
                if (!f.is_open()) return _err("Cannot write to " + s.exportPath);
                f << html;
                f.close();
                return _info("Table '" + s.tableName + "' exported to HTML: " + s.exportPath);
            }
            case AST::EXP_MARKDOWN: {
                std::string md = Export::toMarkdown(db.table());
                return md;
            }
        }
        return _err("Unknown export format.");
    }

    // ── HELP ────────────────────────────────────────────────────────────

    std::string _helpText() const {
        std::ostringstream h;
        h << "\033[1;96m"
          << "╔══════════════════════════════════════════════════════════════════╗\n"
          << "║                    MySQLite Query Language                      ║\n"
          << "╚══════════════════════════════════════════════════════════════════╝\033[0m\n"
          << "\n"
          << "\033[1;93m  Data Loading & Catalog\033[0m\n"
          << "    LOAD \033[36m'file.csv'\033[0m AS \033[33mname\033[0m       Import a CSV file\n"
          << "    SHOW DATABASES                  Show database overview\n"
          << "    SHOW TABLES                     List all loaded tables\n"
          << "    TABLES                          (alias for SHOW TABLES)\n"
          << "    SHOW TABLE \033[33mname\033[0m                (alias for DESCRIBE)\n"
          << "    DESCRIBE \033[33mtable\033[0m                 Show table schema\n"
          << "    DROP TABLE \033[33mtable\033[0m               Remove table from catalog\n"
          << "\n"
          << "\033[1;93m  Queries\033[0m\n"
          << "    SELECT \033[36m*|col,...\033[0m FROM \033[33mtable\033[0m    Query rows\n"
          << "      [WHERE \033[36mcol\033[0m \033[35mop\033[0m \033[36mval\033[0m [AND|OR ...]]  Filter\n"
          << "      [ORDER BY \033[36mcol\033[0m [ASC|DESC]]   Sort\n"
          << "      [LIMIT \033[36mn\033[0m]                   Limit rows\n"
          << "\n"
          << "\033[1;93m  Data Manipulation\033[0m\n"
          << "    INSERT INTO \033[33mtable\033[0m (\033[36mcols\033[0m) VALUES (\033[36mvals\033[0m)\n"
          << "    UPDATE \033[33mtable\033[0m SET \033[36mcol\033[0m=\033[36mval\033[0m [WHERE ...]\n"
          << "    DELETE FROM \033[33mtable\033[0m [WHERE ...]   \n"
          << "\n"
          << "\033[1;93m  Schema Management\033[0m\n"
          << "    CREATE TABLE \033[33mname\033[0m (\033[36mcol type\033[0m, ...)\n"
          << "    ALTER \033[33mtable\033[0m ADD COLUMN \033[36mname\033[0m [\033[35mtype\033[0m]\n"
          << "    ALTER \033[33mtable\033[0m DROP COLUMN \033[36mname\033[0m\n"
          << "    ALTER \033[33mtable\033[0m RENAME COLUMN \033[36mold\033[0m TO \033[36mnew\033[0m\n"
          << "    ALTER \033[33mtable\033[0m MODIFY COLUMN \033[36mcol\033[0m \033[35mtype\033[0m\n"
          << "      Types: STRING, INTEGER, DOUBLE, DATE, BOOLEAN\n"
          << "\n"
          << "\033[1;93m  Aggregation & Statistics\033[0m\n"
          << "    COUNT \033[33mtable\033[0m [WHERE ...]        Count rows\n"
          << "    SUM|AVG|MIN|MAX \033[36mcol\033[0m FROM \033[33mtable\033[0m [WHERE ...]\n"
          << "    STATS \033[36mcol\033[0m FROM \033[33mtable\033[0m          Full statistics\n"
          << "\n"
          << "\033[1;93m  Export & Display\033[0m\n"
          << "    EXPORT \033[33mtable\033[0m TO CSV|HTML \033[36m'path'\033[0m\n"
          << "    EXPORT \033[33mtable\033[0m TO MARKDOWN      Print as Markdown\n"
          << "    STYLE \033[36mname\033[0m                    ocean|matrix|fire|corporate|\n"
          << "                                  newspaper|elegant|minimal\n"
          << "\n"
          << "\033[1;93m  Control\033[0m\n"
          << "    HELP                           Show this message\n"
          << "    QUIT | EXIT                    Leave the REPL\n"
          << "\n"
          << "\033[90m  Operators: =  !=  <  >  <=  >=  |  Strings: 'quoted'  |  ;optional\033[0m\n";
        return h.str();
    }
};

// ════════════════════════════════════════════════════════════════════════
//  REPL Shell — the public entry point
// ════════════════════════════════════════════════════════════════════════

class Repl {
public:
    Repl() : _historyFile(_getHistoryPath()) {
        // Initialise readline
        rl_readline_name = const_cast<char*>("mysqlite");
        using_history();
        stifle_history(1000);
        read_history(_historyFile.c_str());
    }

    ~Repl() {
        write_history(_historyFile.c_str());
    }

    // Pre-load a table before entering the loop
    void preload(const std::string& name, const Database& db) {
        _executor.addTable(name, db);
    }

    // Main loop — uses GNU readline for line editing and history
    void run() {
        _printBanner();

        // Install SIGINT handler (Ctrl+C clears line, does not exit)
        struct sigaction sa, oldSa;
        std::memset(&sa, 0, sizeof(sa));
        sa.sa_handler = _detail::_sigintHandler;
        sa.sa_flags = 0;
        sigemptyset(&sa.sa_mask);
        sigaction(SIGINT, &sa, &oldSa);

        static const char* PROMPT_MAIN = "\001\033[1;96m\002mysqlite\001\033[0m\033[96m\002>\001\033[0m\002 ";
        static const char* PROMPT_CONT = "\001\033[96m\002      ->\001\033[0m\002 ";

        std::string buffer;          // accumulates multi-line input

        while (true) {
            _detail::g_sigint = 0;

            const char* prompt = buffer.empty() ? PROMPT_MAIN : PROMPT_CONT;
            char* raw = readline(prompt);

            // Ctrl+C pressed — discard accumulated buffer, start fresh
            if (_detail::g_sigint) {
                _detail::g_sigint = 0;
                if (raw) free(raw);
                if (!buffer.empty()) {
                    buffer.clear();
                    std::cout << std::endl;
                }
                std::cout << "\033[90m^C\033[0m" << std::endl;
                continue;
            }

            // Ctrl+D (EOF)
            if (!raw) {
                if (!buffer.empty()) {
                    // Discard incomplete statement
                    std::cout << std::endl;
                    buffer.clear();
                    continue;
                }
                std::cout << "\n\033[96mGoodbye.\033[0m" << std::endl;
                break;
            }

            std::string line(raw);
            free(raw);

            // Append to buffer (with space separator for multi-line)
            if (!buffer.empty())
                buffer += " ";
            buffer += line;

            // Check if the statement is terminated with ';'
            std::string trimmed = _trim(buffer);
            if (trimmed.empty()) {
                buffer.clear();
                continue;
            }

            // Not terminated yet — keep reading
            if (trimmed[trimmed.size() - 1] != ';')
                continue;

            // We have a complete statement — add full statement to history
            add_history(trimmed.c_str());

            // Strip the trailing ';'
            std::string stmt_str = _trim(trimmed.substr(0, trimmed.size() - 1));
            buffer.clear();

            if (stmt_str.empty()) continue;

            try {
                // Lexer
                Lexer lexer(stmt_str);
                std::vector<Token> tokens = lexer.tokenize();

                // Parser
                Parser parser(tokens);
                AST::Statement stmt = parser.parse();

                // Quit?
                if (stmt.type == AST::STMT_QUIT) {
                    std::cout << "\033[96mGoodbye.\033[0m" << std::endl;
                    break;
                }

                // Execute
                std::string output = _executor.execute(stmt);
                if (!output.empty())
                    std::cout << output << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "\033[91m✗ Error: " << e.what() << "\033[0m" << std::endl;
            }
        }

        // Restore original signal handler
        sigaction(SIGINT, &oldSa, NULL);

        // Persist history on exit
        write_history(_historyFile.c_str());
    }

private:
    Executor    _executor;
    std::string _historyFile;

    static std::string _getHistoryPath() {
        const char* home = getenv("HOME");
        if (home) return std::string(home) + "/.mysqlite_history";
        return ".mysqlite_history";
    }

    static std::string _trim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    void _printBanner() const {
        std::cout
            << "\033[1;96m"
            << " ╔═══════════════════════════════════════════════════════╗\n"
            << " ║           MySQLite Interactive Shell v1.0            ║\n"
            << " ║       Lexer → Parser → AST → Executor pipeline      ║\n"
            << " ╠═══════════════════════════════════════════════════════╣\n"
            << " ║  Type \033[93mHELP\033[1;96m for commands  •  \033[93mQUIT;\033[1;96m to exit            ║\n"
            << " ║  ↑↓ history  •  readline  •  Ctrl-C / Ctrl-D       ║\n"
            << " ║  Statements must end with \033[93m;\033[1;96m (semicolon)            ║\n"
            << " ╚═══════════════════════════════════════════════════════╝\n"
            << "\033[0m" << std::endl;
    }
};

} // namespace MySQLite

#endif // MYSQLITE_REPL_HPP
