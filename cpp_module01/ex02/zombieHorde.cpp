/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombieHorde.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:38 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 05:45:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"
#include <new>
#include <sstream>

void	destroyHorde(Zombie *horde, int N);

Zombie* zombieHorde( int N, std::string name )
{
	Zombie  			*horde;
	void				*raw;
	int					constructed;

	if (N <= 0)
		return (NULL);
	raw = operator new(sizeof(Zombie) * N);
	horde = static_cast<Zombie*>(raw);
	constructed = 0;
	try
	{
		for (int i = 0; i < N; ++i)
		{
			std::ostringstream	ss;
			ss << name << i;
			new (horde + i)Zombie(ss.str());
			++constructed;
		}
	}
	catch (...)
	{
		destroyHorde(horde, constructed);
		throw;
	}
	return(horde);
}

void	destroyHorde(Zombie *horde, int N)
{
	if (!horde)
		return ;
	for (int i = N - 1; i >= 0; --i)
		(horde + i)->~Zombie();
	operator delete(static_cast<void*>(horde));
}
