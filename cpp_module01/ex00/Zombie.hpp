/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:14 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 06:01:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE01_EX00_ZOMBIE_HPP
# define CPP_MODULE01_EX00_ZOMBIE_HPP

#include <iostream>
#include <string>

class Zombie
{
 public:
	Zombie(const std::string &name, bool heap = false);
	void	announce(void);
	void	setName(const std::string &name);
	~Zombie();
	static const char* C_RESET;
	static const char* C_RED;
	static const char* C_GREEN;
	static const char* C_YELLOW;
	static const char* C_CYAN;

 private:
	std::string name;
	bool allocatedOnHeap;
};

#endif  // CPP_MODULE01_EX00_ZOMBIE_HPP
