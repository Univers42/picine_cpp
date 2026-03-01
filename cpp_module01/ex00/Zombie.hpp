/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 15:59:41 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:40:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE01_EX00_ZOMBIE_HPP_
#define CPP_MODULE01_EX00_ZOMBIE_HPP_

#include <iostream>
#include <string>

// ── UI Styling Macros ────────────────────────────────────────────────────────
#define C_RESET "\033[0m"
#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_CYAN "\033[36m"
#define C_MAGENTA "\033[35m"
#define C_BOLD "\033[1m"
#define C_DIM "\033[2m"

class Zombie {
 private:
  std::string name;  // It has a private string attribute name[cite: 125].

 public:
  explicit Zombie(std::string name);
  ~Zombie();
  void announce(
      void);  // Add a member function void announce(void);[cite: 126].
};

// Standalone functions[cite: 131, 133].
Zombie* newZombie(std::string name);
void randomChump(std::string name);

#endif  // CPP_MODULE01_EX00_ZOMBIE_HPP_
