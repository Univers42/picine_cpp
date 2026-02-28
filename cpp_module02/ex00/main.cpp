/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 18:00:25 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 18:06:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"
#include <iostream>

#define BOLD "\033[1m"
#define MAGENTA "\033[35m"
#define RESET "\033[0m"

int main(void) {
    std::cout << BOLD << MAGENTA << "--- Initialization Phase ---" << RESET << std::endl;
    Fixed a;
    Fixed b(a);
    Fixed c;

    std::cout << BOLD << MAGENTA << "\n--- Assignment Phase ---" << RESET << std::endl;
    c = b;

    std::cout << BOLD << MAGENTA << "\n--- Data Access Phase ---" << RESET << std::endl;
    std::cout << "Value of a: " << a.getRawBits() << std::endl;
    std::cout << "Value of b: " << b.getRawBits() << std::endl;
    std::cout << "Value of c: " << c.getRawBits() << std::endl;

    std::cout << BOLD << MAGENTA << "\n--- Destruction Phase ---" << RESET << std::endl;
    return 0;
}
