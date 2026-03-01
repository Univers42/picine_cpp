/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanB.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:45:20 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:45:47 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE01_EX03_HUMANB_HPP_
#define CPP_MODULE01_EX03_HUMANB_HPP_

#include <string>

#include "Weapon.hpp"

class HumanB {
 private:
  std::string name;
  Weapon* weapon;  // Pointer because HumanB can be unarmed (NULL)

 public:
  explicit HumanB(const std::string& name);
  ~HumanB();

  void setWeapon(Weapon& weapon);
  void attack() const;
};

#endif  // CPP_MODULE01_EX03_HUMANB_HPP_
