/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:02:21 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:37:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"
#include <iostream>
#include <string>

Zombie::Zombie(std::string name) : name(name) {
  std::cout << C_DIM << "  [System] " << C_GREEN << "Zombie '" << this->name
            << "' crawled out of the grave." << C_RESET << std::endl;
}

Zombie::~Zombie() {
  // The destructor must print a message with the name of the zombie.
  std::cout << C_DIM << "  [System] " << C_RED << "Zombie '" << this->name
            << "' turned to dust." << C_RESET << std::endl;
}

void Zombie::announce(void) {
  // Must print: <name>: BraiiiiiiinnnzzzZ... [cite: 127]
  std::cout << C_BOLD << C_CYAN << this->name << C_RESET
            << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
