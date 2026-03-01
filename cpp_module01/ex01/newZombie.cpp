/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newZombie.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:05:03 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:41:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "Zombie.hpp"

// This function creates a zombie, names it, and returns it[cite: 132].
// It uses 'new' to allocate on the heap.
Zombie* newZombie(std::string name) { return new Zombie(name); }
