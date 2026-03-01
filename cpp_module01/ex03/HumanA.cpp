/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:35:30 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:46:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanA.hpp"

#include <iostream>
#include <string>

// O(1) Init list to bind the reference instantly upon creation
HumanA::HumanA(const std::string& name, Weapon& weapon)
    : name(name), weapon(weapon) {}

HumanA::~HumanA() {}

void HumanA::attack() const {
  std::cout << this->name << " attacks with their " << this->weapon.getType()
            << std::endl;
}
