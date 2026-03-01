/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 03:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/01 13:20:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitCoinExchange.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>

// ============================================================================
// Orthodox Canonical Form
// ============================================================================

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const std::string &dbPath)
{
	loadDatabase(dbPath);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	: _priceDb(other._priceDb) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_priceDb = other._priceDb;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

// ============================================================================
// Database loading - uses CSV::Document from vendor
// ============================================================================

void BitcoinExchange::loadDatabase(const std::string &dbPath)
{
	CSV::Parser::Options opts;
	opts.delimiter = ',';
	opts.hasHeader = true;
	opts.trimWhitespace = true;
	opts.skipEmptyLines = true;

	CSV::Document doc;
	if (!doc.load(dbPath, opts))
		throw std::runtime_error("Error: could not open database file.");

	int dateIdx = doc.getColumnIndex("date");
	int priceIdx = doc.getColumnIndex("exchange_rate");
	if (dateIdx < 0 || priceIdx < 0)
		throw std::runtime_error("Error: invalid database format.");

	for (size_t i = 0; i < doc.rowCount(); ++i)
	{
		std::string date = doc.rows()[i][dateIdx];
		std::string price = doc.rows()[i][priceIdx];
		char *endptr = NULL;
		double val = std::strtod(price.c_str(), &endptr);
		if (endptr != NULL && endptr != price.c_str())
			_priceDb[date] = val;
	}
}

// ============================================================================
// Input file processing
// ============================================================================

void BitcoinExchange::processInputFile(const std::string &inputPath) const
{
	std::ifstream file;
	file.open(inputPath.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	bool firstLine = true;
	while (std::getline(file, line))
	{
		if (firstLine)
		{
			firstLine = false;
			// Skip header line (expected: "date | value")
			std::string trimmed = _trim(line);
			if (trimmed.find("date") != std::string::npos
				&& trimmed.find("value") != std::string::npos)
				continue;
		}

		// Parse "date | value"
		size_t pipePos = line.find('|');
		if (pipePos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << _trim(line) << std::endl;
			continue;
		}

		std::string dateStr = _trim(line.substr(0, pipePos));
		std::string valueStr = _trim(line.substr(pipePos + 1));

		// Validate date
		if (!_isValidDate(dateStr))
		{
			std::cerr << "Error: bad input => " << dateStr << std::endl;
			continue;
		}

		// Validate value
		double value = 0;
		if (!_isValidValue(valueStr, value))
			continue;

		// Get exchange rate and output
		double rate = _getExchangeRate(dateStr);
		if (rate < 0)
		{
			std::cerr << "Error: date too early for database." << std::endl;
			continue;
		}

		double result = value * rate;
		std::cout << dateStr << " => " << valueStr << " = " << result << std::endl;
	}
}

// ============================================================================
// Date validation - uses Date class from vendor
// ============================================================================

bool BitcoinExchange::_isValidDate(const std::string &date) const
{
	if (date.length() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;

	// Check all other chars are digits
	for (size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return false;
	}

	try
	{
		Date d = Date::parse(date, "YYYY-MM-DD");
		return d.isValid();
	}
	catch (const std::exception &)
	{
		return false;
	}
}

// ============================================================================
// Value validation
// ============================================================================

bool BitcoinExchange::_isValidValue(const std::string &value, double &out) const
{
	if (value.empty())
	{
		std::cerr << "Error: bad input => " << value << std::endl;
		return false;
	}

	char *endptr = NULL;
	out = std::strtod(value.c_str(), &endptr);

	if (endptr == value.c_str() || (endptr != NULL && *endptr != '\0'))
	{
		std::cerr << "Error: bad input => " << value << std::endl;
		return false;
	}

	if (out < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	if (out > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}
	return true;
}

// ============================================================================
// Exchange rate lookup - uses std::map::lower_bound
// ============================================================================

double BitcoinExchange::_getExchangeRate(const std::string &date) const
{
	if (_priceDb.empty())
		return -1;

	// Try exact match first
	std::map<std::string, double>::const_iterator it = _priceDb.find(date);
	if (it != _priceDb.end())
		return it->second;

	// Find closest lower date using upper_bound then stepping back
	it = _priceDb.upper_bound(date);
	if (it == _priceDb.begin())
		return -1; // date is before earliest entry
	--it;
	return it->second;
}

// ============================================================================
// Utility
// ============================================================================

std::string BitcoinExchange::_trim(const std::string &s)
{
	size_t start = s.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return "";
	size_t end = s.find_last_not_of(" \t\r\n");
	return s.substr(start, end - start + 1);
}

// ============================================================================
// MySQLite rich rendering mode
// ============================================================================

#if HAVE_MY_SQL_LITE

RenderConfig BitcoinExchange::_getStyle(const std::string &name)
{
	if (name == "matrix")
		return StylePresets::matrix();
	if (name == "fire")
		return StylePresets::fire();
	if (name == "corporate")
		return StylePresets::corporate();
	if (name == "newspaper")
		return StylePresets::newspaper();
	if (name == "elegant")
		return StylePresets::elegant();
	if (name == "minimal")
		return RenderConfig::minimal();
	// default: ocean
	return StylePresets::ocean();
}

// ──────────────────────────────────────────────────────────────────────
// processAndRender: parse input, evaluate, build a Table, render it
// ──────────────────────────────────────────────────────────────────────

void BitcoinExchange::processAndRender(const std::string &inputPath,
	const std::string &styleName) const
{
	std::ifstream file;
	file.open(inputPath.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	// Build a Database table with columns: Date | Value | Rate | Result | Status
	Database db;
	db.addColumn("Date",   ColumnType::STRING,  Alignment::LEFT);
	db.addColumn("Value",  ColumnType::DOUBLE,  Alignment::RIGHT);
	db.addColumn("Rate",   ColumnType::DOUBLE,  Alignment::RIGHT);
	db.addColumn("Result", ColumnType::DOUBLE,  Alignment::RIGHT);
	db.addColumn("Status", ColumnType::STRING,  Alignment::LEFT);

	std::string line;
	bool firstLine = true;
	size_t okCount = 0;
	size_t errCount = 0;

	while (std::getline(file, line))
	{
		if (firstLine)
		{
			firstLine = false;
			std::string trimmed = _trim(line);
			if (trimmed.find("date") != std::string::npos
				&& trimmed.find("value") != std::string::npos)
				continue;
		}

		std::map<std::string, std::string> rowData;

		size_t pipePos = line.find('|');
		if (pipePos == std::string::npos)
		{
			rowData["Date"]   = _trim(line);
			rowData["Value"]  = "";
			rowData["Rate"]   = "";
			rowData["Result"] = "";
			rowData["Status"] = "bad input";
			db.addRow(rowData);
			++errCount;
			continue;
		}

		std::string dateStr  = _trim(line.substr(0, pipePos));
		std::string valueStr = _trim(line.substr(pipePos + 1));

		rowData["Date"] = dateStr;

		if (!_isValidDate(dateStr))
		{
			rowData["Value"]  = valueStr;
			rowData["Rate"]   = "";
			rowData["Result"] = "";
			rowData["Status"] = "bad date";
			db.addRow(rowData);
			++errCount;
			continue;
		}

		double value = 0;
		if (valueStr.empty())
		{
			rowData["Value"]  = "";
			rowData["Rate"]   = "";
			rowData["Result"] = "";
			rowData["Status"] = "bad input";
			db.addRow(rowData);
			++errCount;
			continue;
		}

		char *endptr = NULL;
		value = std::strtod(valueStr.c_str(), &endptr);
		if (endptr == valueStr.c_str() || (endptr != NULL && *endptr != '\0'))
		{
			rowData["Value"]  = valueStr;
			rowData["Rate"]   = "";
			rowData["Result"] = "";
			rowData["Status"] = "bad value";
			db.addRow(rowData);
			++errCount;
			continue;
		}
		if (value < 0)
		{
			rowData["Value"]  = valueStr;
			rowData["Rate"]   = "";
			rowData["Result"] = "";
			rowData["Status"] = "negative";
			db.addRow(rowData);
			++errCount;
			continue;
		}
		if (value > 1000)
		{
			rowData["Value"]  = valueStr;
			rowData["Rate"]   = "";
			rowData["Result"] = "";
			rowData["Status"] = "too large";
			db.addRow(rowData);
			++errCount;
			continue;
		}

		double rate = _getExchangeRate(dateStr);
		if (rate < 0)
		{
			rowData["Value"]  = valueStr;
			rowData["Rate"]   = "";
			rowData["Result"] = "";
			rowData["Status"] = "no rate";
			db.addRow(rowData);
			++errCount;
			continue;
		}

		double result = value * rate;
		{
			std::ostringstream vs, rs, res;
			vs << value;
			rs << rate;
			res << result;
			rowData["Value"]  = vs.str();
			rowData["Rate"]   = rs.str();
			rowData["Result"] = res.str();
		}
		rowData["Status"] = "OK";
		db.addRow(rowData);
		++okCount;
	}

	// Configure style & value-based coloring
	RenderConfig cfg = _getStyle(styleName);
	cfg.autoIncrementId = true;
	cfg.showFooter = true;
	{
		std::ostringstream footer;
		footer << okCount << " OK | " << errCount << " errors | "
			<< (okCount + errCount) << " total";
		cfg.footerText = footer.str();
	}
	cfg.footerStyle.foreground = Style::Color::BrightYellow();
	cfg.footerStyle.bold = true;

	// Value-based styling for Status column
	Style::CellStyle okStyle;
	okStyle.foreground = Style::Color::BrightGreen();
	okStyle.bold = true;
	cfg.value_styles["ok"] = okStyle;

	Style::CellStyle errStyle;
	errStyle.foreground = Style::Color::BrightRed();
	errStyle.bold = true;
	cfg.value_styles["bad input"]  = errStyle;
	cfg.value_styles["bad date"]   = errStyle;
	cfg.value_styles["bad value"]  = errStyle;
	cfg.value_styles["negative"]   = errStyle;
	cfg.value_styles["too large"]  = errStyle;
	cfg.value_styles["no rate"]    = errStyle;

	std::cout << db.render(cfg) << std::endl;
}

// ──────────────────────────────────────────────────────────────────────
// showDatabase: render the price DB as a styled table
// ──────────────────────────────────────────────────────────────────────

void BitcoinExchange::showDatabase(const std::string &styleName,
	size_t maxRows) const
{
	Database db;
	db.addColumn("Date",          ColumnType::DATE,   Alignment::LEFT);
	db.addColumn("Exchange Rate", ColumnType::DOUBLE,  Alignment::RIGHT);

	for (std::map<std::string, double>::const_iterator it = _priceDb.begin();
		it != _priceDb.end(); ++it)
	{
		std::map<std::string, std::string> row;
		row["Date"] = it->first;
		{
			std::ostringstream oss;
			oss << it->second;
			row["Exchange Rate"] = oss.str();
		}
		db.addRow(row);
	}

	Table tbl = db.table();
	if (maxRows > 0)
		tbl = Transform::limit(tbl, maxRows);

	RenderConfig cfg = _getStyle(styleName);
	cfg.autoIncrementId = true;
	cfg.showFooter = true;
	{
		std::ostringstream footer;
		footer << "Bitcoin Price Database — " << _priceDb.size() << " entries";
		cfg.footerText = footer.str();
	}
	cfg.footerStyle.foreground = Style::Color::BrightCyan();
	cfg.footerStyle.bold = true;

	// Color the Rate column
	cfg.color_column["Exchange Rate"] = Style::Color::BrightYellow();

	TableRenderer renderer(cfg);
	std::cout << renderer.render(tbl) << std::endl;
}

// ──────────────────────────────────────────────────────────────────────
// showStats: compute and display statistics on exchange_rate
// ──────────────────────────────────────────────────────────────────────

void BitcoinExchange::showStats() const
{
	Database db;
	db.addColumn("Date",          ColumnType::DATE,   Alignment::LEFT);
	db.addColumn("Exchange Rate", ColumnType::DOUBLE,  Alignment::RIGHT);

	for (std::map<std::string, double>::const_iterator it = _priceDb.begin();
		it != _priceDb.end(); ++it)
	{
		std::map<std::string, std::string> row;
		row["Date"] = it->first;
		{
			std::ostringstream oss;
			oss << it->second;
			row["Exchange Rate"] = oss.str();
		}
		db.addRow(row);
	}

	Statistics::ColumnStats stats = Statistics::analyze(db.table(), "Exchange Rate");
	Database statsDb = Statistics::createStatsTable(stats, "Exchange Rate");

	RenderConfig cfg = StylePresets::corporate();
	cfg.autoIncrementId = false;
	cfg.showFooter = true;
	cfg.footerText = "BTC Exchange Rate Statistics";
	cfg.footerStyle.foreground = Style::Color::BrightCyan();
	cfg.footerStyle.bold = true;

	std::cout << statsDb.render(cfg) << std::endl;
}

#endif // HAVE_MY_SQL_LITE
