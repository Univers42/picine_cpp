/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 15:59:41 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 16:31:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

# include <string>
# include <iostream>

// ── UI Styling Macros ────────────────────────────────────────────────────────
# define C_RESET   "\033[0m"
# define C_RED     "\033[31m"
# define C_GREEN   "\033[32m"
# define C_YELLOW  "\033[33m"
# define C_CYAN    "\033[36m"
# define C_MAGENTA "\033[35m"
# define C_BOLD    "\033[1m"
# define C_DIM     "\033[2m"

class Zombie {
 private:
	std::string name; // Private string attribute

 public:
	Zombie(); // Default constructor needed for array allocation
	Zombie(std::string name);
	~Zombie();

	void announce(void);
	void setName(std::string name); // Setter to initialize the horde
};

// Standalone function required by the subject
Zombie* zombieHorde(int N, std::string name);

#endif
