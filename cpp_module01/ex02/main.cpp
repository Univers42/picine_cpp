/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:31 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 05:45:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

Zombie* zombieHorde(int N, std::string name);
void destroyHorde(Zombie *horde, int N);

int	main(void)
{
	int N = 5;
	Zombie *horde = zombieHorde(N, "Zombie");
	if (horde)
	{
		for (int i = 0; i < N; ++i)
			horde[i].announce();
		destroyHorde(horde, N);
	}
	return (0);
}
