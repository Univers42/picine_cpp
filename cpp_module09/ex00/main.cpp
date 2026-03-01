/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 03:00:00 by marvin            #+#    #+#             */
/*   Updated: 2026/03/01 13:35:13 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitCoinExchange.hpp"
#include <iostream>
#include <cstring>

#if HAVE_MY_SQL_LITE
# include "vendor/MySQLiteRepl.hpp"

static void printHelp(void)
{
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

int main(int argc, char **argv)
{
	std::string inputFile;
	std::string style     = "ocean";
	bool showDb           = false;
	bool showStats        = false;
	bool classicMode      = false;
	bool interactive      = false;
	size_t dbLimit        = 0;

	for (int i = 1; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--help" || arg == "-h")
		{
			printHelp();
			return 0;
		}
		else if (arg == "--style" && i + 1 < argc)
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
		else
		{
			std::cerr << "Unknown option: " << arg << std::endl;
			printHelp();
			return 1;
		}
	}

	if (inputFile.empty() && !showDb && !showStats && !interactive)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	try
	{
		BitcoinExchange btc("data.csv");

		if (interactive)
		{
			// Pre-load the bitcoin price DB into the REPL catalog
			Database priceDb;
			priceDb.loadFromCsv("data.csv");
			MySQLite::Repl repl;
			repl.preload("btc", priceDb);
			repl.run();
			return 0;
		}

		if (showDb)
			btc.showDatabase(style, dbLimit);
		if (showStats)
			btc.showStats();
		if (!inputFile.empty())
		{
			if (classicMode)
				btc.processInputFile(inputFile);
			else
				btc.processAndRender(inputFile, style);
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}

#else // Classic 42 mode — no table rendering

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: could not open file." << std::endl;
		return 1;
	}

	try
	{
		BitcoinExchange btc("data.csv");
		btc.processInputFile(argv[1]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}

#endif
