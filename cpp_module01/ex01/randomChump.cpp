/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomChump.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:06:01 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:42:11 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Zombie.hpp"

// This function creates a zombie, names it, and makes it announce itself[cite:
// 134]. It allocates directly on the stack.
void randomChump(std::string name) {
  Zombie stackZombie(name);
  stackZombie.announce();
}
