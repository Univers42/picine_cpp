/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitCoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 03:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/01 13:19:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <string>

#include "vendor/Date.hpp"
#include "vendor/csv.hpp"

#if HAVE_MY_SQL_LITE
#include "vendor/Database.hpp"
#include "vendor/Database_utils.hpp"
#endif

class BitcoinExchange {
 public:
  BitcoinExchange();
  BitcoinExchange(const std::string &dbPath);
  BitcoinExchange(const BitcoinExchange &other);
  BitcoinExchange &operator=(const BitcoinExchange &other);
  ~BitcoinExchange();

  void loadDatabase(const std::string &dbPath);
  void processInputFile(const std::string &inputPath) const;

#if HAVE_MY_SQL_LITE
  // ── MySQLite rich mode ──────────────────────────────────────────────
  // Processes the input file and renders a styled table with results
  void processAndRender(const std::string &inputPath,
                        const std::string &styleName = "ocean") const;
  // Show the raw price database as a rendered table
  void showDatabase(const std::string &styleName = "ocean",
                    size_t maxRows = 0) const;
  // Show statistics about the price database
  void showStats() const;
#endif

 private:
  std::map<std::string, double> _priceDb;

  bool _isValidDate(const std::string &date) const;
  bool _isValidValue(const std::string &value, double &out) const;
  double _getExchangeRate(const std::string &date) const;
  static std::string _trim(const std::string &s);

#if HAVE_MY_SQL_LITE
  static RenderConfig _getStyle(const std::string &name);
#endif
};

#endif
