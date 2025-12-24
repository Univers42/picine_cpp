/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 02:00:44 by marvin            #+#    #+#             */
/*   Updated: 2025/12/24 02:00:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VALIDATOR_HPP
# define VALIDATOR_HPP

class Validator
{
	public:
		static void isValidValue(double value);
		static bool isValidEmail(std::string const& email);
		static bool isValidPhoneNumber(std::string const& phoneNumber);
		static bool isValidDate(std::string const& date);
	private:
		Validator();
};
#endif