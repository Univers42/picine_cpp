/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:10 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 05:45:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"
#include <iostream>

// define static color constants (C++98-compatible)
const char* Zombie::C_RESET  = "\033[0m";
const char* Zombie::C_RED    = "\033[31m";
const char* Zombie::C_GREEN  = "\033[32m";
const char* Zombie::C_YELLOW = "\033[33m";
const char* Zombie::C_CYAN   = "\033[36m";

// Const ref + init list
Zombie::Zombie(const std::string &name, bool heap) : name(name), allocatedOnHeap(heap)
{
	// clean creation message
	std::cout << (allocatedOnHeap ? Zombie::C_YELLOW : Zombie::C_GREEN)
	          << "+ " << name << (allocatedOnHeap ? " [heap]" : " [stack]")
	          << Zombie::C_RESET << std::endl;
}

Zombie::~Zombie()
{
	// clean destruction message
	std::cout << Zombie::C_RED
	          << "- " << name << (allocatedOnHeap ? " [heap freed]" : " [destroyed]")
	          << Zombie::C_RESET << std::endl;
}

void Zombie::announce(void)
{
	// professional, clean announcement
	std::cout << C_CYAN << name << C_RESET
	          << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

void Zombie::setName(const std::string &n)
{
	name = n;
}
