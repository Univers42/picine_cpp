/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:01 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 05:45:02 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

Zombie  *newZombie( const std::string& name );
void    randomChump( const std::string& name );

int main(void)
{
	std::cout << Zombie::C_CYAN
	          << "Zombie Memory Management Demo\n"
	          << "-----------------------------\n"
	          << Zombie::C_RESET << std::endl;

	Zombie	*zombie;
	const std::string	list[] =
	{
		"Elena",
		"Gabriel",
		"Juan",
		"Cris",
		"Susana"
	};

	std::srand(static_cast<unsigned int>(std::time(NULL)));
	const size_t n = sizeof(list) / sizeof(list[0]);
	zombie = newZombie("Charlie");
	zombie->announce();
	for (size_t i = 0; i < n; i++)
		randomChump(list[std::rand() % n]);
	delete zombie;
	return (0);
}
