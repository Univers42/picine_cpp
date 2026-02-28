/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:02:21 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 16:31:51 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

// Default constructor called when allocating the array of N zombies
Zombie::Zombie(void) : name("Unnamed") {
	std::cout << C_DIM << "  [System] " << C_GREEN << "An unnamed Zombie crawled out of the grave." << C_RESET << std::endl;
}

Zombie::Zombie(std::string name) : name(name) {
	std::cout << C_DIM << "  [System] " << C_GREEN << "Zombie '" << this->name
            << "' crawled out of the grave." << C_RESET << std::endl;
}

Zombie::~Zombie() {
	std::cout << C_DIM << "  [System] " << C_RED << "Zombie '" << this->name
            << "' turned to dust." << C_RESET << std::endl;
}

void Zombie::announce(void) {
	std::cout << C_BOLD << C_CYAN << this->name << C_RESET
            << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

// Setter function to give names to the horde
void Zombie::setName(std::string name) {
	this->name = name;
}
