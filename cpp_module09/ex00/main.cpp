/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 03:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/01 16:15:09 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>
#include <fstream>
#include <iostream>

#include "BitCoinExchange.hpp"

#if HAVE_MY_SQL_LITE
#include "vendor/MySQLiteRepl.hpp"

static void printHelp(void) {
  std::cout
      << "Usage: btc [OPTIONS] <input_file>\n"
      << "\n"
      << "OPTIONS:\n"
      << "  --style <name>   Table style: ocean, matrix, fire, corporate,\n"
      << "                   newspaper, elegant, minimal  (default: ocean)\n"
      << "  --db             Show the price database as a table\n"
      << "  --db-limit <n>   Limit DB rows shown (default: all)\n"
      << "  --stats          Show statistics on exchange rates\n"
      << "  --classic        Run classic 42 output (no table rendering)\n"
      << "  -i, --interactive  Launch MySQLite interactive REPL shell\n"
      << "  --help           Show this message\n";
}

int main(int argc, char **argv) {
  std::string inputFile;
  std::string style = "ocean";
  bool showDb = false;
  bool showStats = false;
  bool classicMode = false;
  bool interactive = false;
  size_t dbLimit = 0;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printHelp();
      return 0;
    } else if (arg == "--style" && i + 1 < argc)
      style = argv[++i];
    else if (arg == "--db")
      showDb = true;
    else if (arg == "--db-limit" && i + 1 < argc)
      dbLimit = std::atoi(argv[++i]);
    else if (arg == "--stats")
      showStats = true;
    else if (arg == "--classic")
      classicMode = true;
    else if (arg == "--interactive" || arg == "-i")
      interactive = true;
    else if (arg[0] != '-')
      inputFile = arg;
    else {
      std::cerr << "Unknown option: " << arg << std::endl;
      printHelp();
      return 1;
    }
  }

  if (inputFile.empty() && !showDb && !showStats && !interactive) {
    std::cerr << "Error: could not open file." << std::endl;
    return 1;
  }

  try {
    if (interactive) {
      // Interactive REPL — data.csv is optional, not required
      Repl repl;
      // Try to pre-load the bitcoin price DB if available
      std::ifstream testFile("data.csv");
      if (testFile.is_open()) {
        testFile.close();
        Database priceDb;
        priceDb.loadFromCsv("data.csv");
        repl.preload("btc", priceDb);
      }
      repl.run();
      return 0;
    }

    BitcoinExchange btc("data.csv");

    if (showDb) btc.showDatabase(style, dbLimit);
    if (showStats) btc.showStats();
    if (!inputFile.empty()) {
      if (classicMode)
        btc.processInputFile(inputFile);
      else
        btc.processAndRender(inputFile, style);
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

#else  // Classic 42 mode — no table rendering

static void printUsage(void) {
  std::cerr
      << "Usage: btc <input_file>\n"
      << "\n"
      << "  Reads date | value lines from <input_file> and prints the\n"
      << "  corresponding BTC value based on the data.csv price database.\n"
      << "\n"
      << "  Build with `make sqlite` to unlock the full interactive\n"
      << "  MySQLite REPL, styled table rendering, and more options.\n"
      << "\n"
      << "OPTIONS (classic build):\n"
      << "  -h, --help   Show this message\n";
}

int main(int argc, char **argv) {
  std::string inputFile;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--help" || arg == "-h") {
      printUsage();
      return 0;
    } else if (arg == "--interactive" || arg == "-i") {
      std::cerr << "Interactive mode requires the MySQLite build.\n"
                << "Rebuild with:  make sqlite\n";
      return 1;
    } else if (arg[0] != '-') {
      inputFile = arg;
    } else {
      std::cerr << "Unknown option: " << arg << "\n";
      printUsage();
      return 1;
    }
  }

  if (inputFile.empty()) {
    std::cerr << "Error: could not open file." << std::endl;
    return 1;
  }

  try {
    BitcoinExchange btc("data.csv");
    btc.processInputFile(inputFile);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

#endif
