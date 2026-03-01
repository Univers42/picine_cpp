/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:41:20 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:45:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE01_EX03_HUMANA_HPP_
#define CPP_MODULE01_EX03_HUMANA_HPP_

#include <string>

#include "Weapon.hpp"

class HumanA {
 private:
  std::string name;
  Weapon& weapon;  // Reference because HumanA is ALWAYS armed

 public:
  HumanA(const std::string& name, Weapon& weapon);
  ~HumanA();
  void attack() const;
};

#endif  // CPP_MODULE01_EX03_HUMANA_HPP_
