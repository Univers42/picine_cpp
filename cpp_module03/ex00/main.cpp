/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:55:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/13 23:02:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

#include "ClapTrap.hpp"
#include <iostream>


#include "ClapTrap.hpp"
#include <iostream>

int main()
{
    std::cout << "=== Construction ===" << std::endl;
    ClapTrap a("Alpha");
    ClapTrap b("Bravo");

    std::cout << "\n=== Copy Construction ===" << std::endl;
    ClapTrap c(b);

    std::cout << "\n=== Move Construction ===" << std::endl;
    ClapTrap d(std::move(a));

    std::cout << "\n=== Copy Assignment ===" << std::endl;
    c = b;

    std::cout << "\n=== Move Assignment ===" << std::endl;
    d = ClapTrap("Delta");

    std::cout << "\n=== Gameplay ===" << std::endl;
    b.attack("Delta");
    d.takeDamage(0);

    d.attack("Bravo");
    b.takeDamage(0);

    b.beRepaired(3);
    d.beRepaired(2);

    std::cout << "\n=== Damage and Repair Edge Cases ===" << std::endl;
    b.takeDamage(20); // Should drop to 0 HP
    b.beRepaired(5);  // Should still work if your logic allows

    std::cout << "\n=== End of Test ===" << std::endl;
    return 0;
}