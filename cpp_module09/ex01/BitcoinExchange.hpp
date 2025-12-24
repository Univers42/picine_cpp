/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:02:11 by marvin            #+#    #+#             */
/*   Updated: 2025/12/24 02:02:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

/**
 * Facade class for bitcoin exchange rate lookyp and processing
 * 
 */
class BitcoinExchange
{
	public:
		static void loadData(std::string const& path);
		void 	    run(std::string const& inputFile);
		static double getRate(std::string const& date);
	private:
		BitcoinExchange();
		ExchangeRateDatabase _db;
};

class ExchangeRateDatabase
{
	public:
		ExchangeRateDatabase();
		void loadFromFile(std::string const& path);
		double getRate(std::string const& date) const;
	private:
		std::map<std::string, double> _rates;
};

class InputLine
{

}

class BitcoinValidator
{

}

class BitcointExchangeApp
{

}

class BitcoinRules
{

}

#endif