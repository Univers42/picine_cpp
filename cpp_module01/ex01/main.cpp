/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:20 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 06:20:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Zombie.hpp"

int main(void) {
   Zombie* horde = Zombie::zombieHorde(5, "Zombie");

   if (horde) {
       for (int i = 0; i < 5; ++i)
           horde[i].announce();
       delete[] horde;
   }
   return 0;
}
