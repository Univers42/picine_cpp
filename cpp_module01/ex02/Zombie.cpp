/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Zombie.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:33 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 05:45:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

Zombie::Zombie( const std::string& name ) : name(name) { }

Zombie::~Zombie( ){ }

void    Zombie::announce() const
{
    std::cout << name << " Braiiiiiiinnnssss..." << std::endl;
}

