/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:39:59 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:46:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Weapon.hpp"
#include <string>

Weapon::Weapon(const std::string& type) : type(type) {}

Weapon::~Weapon() {}

const std::string& Weapon::getType() const { return this->type; }

void Weapon::setType(const std::string& type) { this->type = type; }
