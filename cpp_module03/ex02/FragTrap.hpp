/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FragTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:27:25 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 23:32:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE03_EX02_FRAGTRAP_HPP_
#define CPP_MODULE03_EX02_FRAGTRAP_HPP_

#include <iostream>
#include <string>

#include "ClapTrap.hpp"

class FragTrap : public ClapTrap {  // Fixed the typo here!
 public:
  // Orthodox Canonical Form
  FragTrap();
  FragTrap(const FragTrap& src);
  FragTrap& operator=(const FragTrap& rhs);
  ~FragTrap();

  // Parameterized Constructor
  explicit FragTrap(const std::string& name);

  // Special ability
  void highFivesGuys(void);
};

#endif  // CPP_MODULE03_EX02_FRAGTRAP_HPP_
