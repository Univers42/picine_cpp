/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:57:42 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/13 21:33:43 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

/**
 * PURPOSE: creates a brand-new `claptrap` object.
 * usally set up by default HP, attack damage, etc..
 * runs when  we do ClapTrap a("bob")
 */
ClapTrap::ClapTrap(const std::string& name):
											name(name),
											hit(10),
											nrj(10),
											attak(0)
{
	std::cout << "ClapTrap " << name << " just has been created!" << std::endl;
}

/**
 * PURPOSE: clean up when the object is destroyed
 * Runs automatically when it goes ut of scope or is deleted
 * if we have dynamica memory, release it here
 */
ClapTrap::~ClapTrap()
{
	std::cout << name << "has all been destroyed!" << std::endl;
}

/**
 * PURPOSE: makes a deep copy of another ClapTrap
 * Called when we create a new object from another:
 * ClapTrap c = b;      // cpy ctor
 * // should copy every member so the ew object is independant
 */
ClapTrap::ClapTrap(const ClapTrap& other):
										name(other.name),
										hit(other.hit),
										nrj(other.nrj),
										attak(other.attak)
										
{
	std::cout << "ClapTrap " << name << "copied!" << std::endl;
}

ClapTrap::ClapTrap(ClapTrap&& other):
								name(other.name),
								hit(other.hit),
								nrj(other.nrj),
								attak(other.attak)
{
	std::cout << "ClapTrap" << name << " moved!" << std::endl;
}

ClapTrap& ClapTrap::operator=(const ClapTrap& other)
{
    std::cout << "Copy assignment operator called" << std::endl;
    if (this != &other)
    {
        hit = other.hit;
        nrj = other.nrj;
        attak = other.attak;
    }
    return (*this);
}

ClapTrap& ClapTrap::operator=(ClapTrap&& other)
{
	std::cout << "Move assignment operator called" << std::endl;
	if (this != &other)
	{
		hit = other.hit;
		nrj = other.nrj;
		attak = other.attak;
	}
	return (*this);
}

void ClapTrap::attack(const std::string& target)
{
    if (nrj > 0 && hit > 0)
    {
        std::cout << "ClapTrap " << name << " attacks " << target
                  << ", causing " << attak << " points of damage!" << std::endl;
        nrj--;
    }
    else
        std::cout << "ClapTrap " << name << " can't attack!" << std::endl;
}

void ClapTrap::takeDamage(uint32_t amount)
{
    if (hit == 0)
    {
        std::cout << "ClapTrap " << name << " is already destroyed!" << std::endl;
        return;
    }
    if (amount >= hit)
        hit = 0;
    else
        hit -= amount;
    std::cout << "ClapTrap " << name << " takes " << amount << " points of damage! Remaining HP: " << hit << std::endl;
}

void ClapTrap::beRepaired(uint32_t amount)
{
    if (nrj > 0 && hit > 0)
    {
        hit += amount;
        nrj--;
        std::cout << "ClapTrap " << name << " has repaired itself for " << amount << " HP! Total HP: " << hit << std::endl;
    }
    else
        std::cout << "ClapTrap " << name << " couldn't repair itself!" << std::endl;
}
