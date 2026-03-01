/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:37:31 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:46:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE01_EX03_WEAPON_HPP_
#define CPP_MODULE01_EX03_WEAPON_HPP_

#include <string>

class Weapon {
 private:
  std::string type;

 public:
  explicit Weapon(const std::string& type);
  ~Weapon();

  // Returns a constant reference to type (O(1) memory address return)
  const std::string& getType() const;
  void setType(const std::string& type);
};

#endif  // CPP_MODULE01_EX03_WEAPON_HPP_
