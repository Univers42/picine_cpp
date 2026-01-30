/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:01 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 06:14:02 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>

Zombie  *newZombie( const std::string& name );
void    randomChump( const std::string& name );
void    heap_approach();
void    stack_approach();
void    scope_demo();

/**
 * PURPOSE: lifetime is controlled manually.
 * We use heap when the  object must outlive the current scope,
 * when we need dynamic lifetime, or when allocating large resourcs.
 * We always remember to delete (or use smart pointers) to avoid leaks.
 */
void heap_approach()
{
  Zombie *zombie = newZombie("HeapZombie");
  zombie->announce();
  delete zombie;
}

/**
 * PURPOSE: lifetime is tied to scope.  we use stack when the object is
 * short-lived, small, and only needed inside the current function - allocation
 * is cheap and exception-safe.
 */
void stack_approach()
{
  Zombie zombie("StackZombie");
  zombie.announce();
}

/**
 * Quick demonstration of the two approaches and when to pick each
 *
 */
int main(void)
{
	// Quick demonstration of the two approaches and when to pick each.
	scope_demo();
	stack_approach(); // automatic lifetime
	heap_approach();  // dynamic lifetime

	Zombie	*zombie;
	const std::string	list[] =
	{
		"Elena",
		"Gabriel",
		"Juan",
		"Cris",
		"Susana"
	};

	std::srand(static_cast<unsigned int>(std::time(NULL)));
	const size_t n = sizeof(list) / sizeof(list[0]);
	zombie = newZombie("Charlie");
	zombie->announce();
	for (size_t i = 0; i < n; i++)
		randomChump(list[std::rand() % n]);
	delete zombie;
	return (0);
}
