/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiamondTrap.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:35:38 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 23:38:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE03_EX03_DIAMONDTRAP_HPP_
#define CPP_MODULE03_EX03_DIAMONDTRAP_HPP_

#include <iostream>
#include <string>

#include "FragTrap.hpp"
#include "ScavTrap.hpp"

class DiamondTrap : public ScavTrap, public FragTrap {
 private:
  std::string _name;  // Shadows ClapTrap::_name

 public:
  // Orthodox Canonical Form
  DiamondTrap();
  DiamondTrap(const DiamondTrap& src);
  DiamondTrap& operator=(const DiamondTrap& rhs);
  ~DiamondTrap();

  // Parameterized Constructor
  explicit DiamondTrap(const std::string& name);

  // Expose ScavTrap's attack to resolve multiple inheritance ambiguity
  using ScavTrap::attack;

  // Special ability
  void whoAmI();
};

#endif  // CPP_MODULE03_EX03_DIAMONDTRAP_HPP_
