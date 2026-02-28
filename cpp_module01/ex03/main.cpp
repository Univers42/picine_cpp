/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:41 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 17:19:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HumanA.hpp"
#include "HumanB.hpp"
#include "Weapon.hpp"
#include <iostream>

// ── UI Styling Macros ────────────────────────────────────────────────────────
#define C_RESET   "\033[0m"
#define C_BOLD    "\033[1m"
#define C_CYAN    "\033[36m"
#define C_MAGENTA "\033[35m"
#define C_YELLOW  "\033[33m"
#define C_GREEN   "\033[32m"
#define C_DIM     "\033[2m"

int main() {
  std::cout << C_BOLD << C_MAGENTA << "\n=== 1. MANDATORY SUBJECT TESTS ===" << C_RESET << std::endl;
  {
    std::cout << C_BOLD << C_CYAN << "\n[HumanA Test]" << C_RESET << std::endl;
    Weapon club = Weapon("crude spiked club");
    HumanA bob("Bob", club);
    bob.attack();

    std::cout << C_DIM << "(Changing club type...)" << C_RESET << std::endl;
    club.setType("some other type of club");
    bob.attack();
  }
  {
    std::cout << C_BOLD << C_CYAN << "\n[HumanB Test]" << C_RESET << std::endl;
    Weapon club = Weapon("crude spiked club");
    HumanB jim("Jim");
    jim.setWeapon(club);
    jim.attack();

    std::cout << C_DIM << "(Changing club type...)" << C_RESET << std::endl;
    club.setType("some other type of club");
    jim.attack();
  }

  std::cout << C_BOLD << C_MAGENTA << "\n=== 2. EDGE CASE: HUMAN_B UNARMED ===" << C_RESET << std::endl;
  {
    HumanB paul("Pacifist Paul");

    std::cout << C_YELLOW << "-> Paul tries to attack without a weapon:" << C_RESET << std::endl;
    // If you didn't check for NULL in your attack() function, this line would cause a Segfault!
    paul.attack();

    std::cout << C_YELLOW << "-> Paul finds a shiny longsword!" << C_RESET << std::endl;
    Weapon sword("shiny longsword");
    paul.setWeapon(sword);
    paul.attack();
  }

  std::cout << C_BOLD << C_MAGENTA << "\n=== 3. EDGE CASE: EMPTY STRINGS ===" << C_RESET << std::endl;
  {
    Weapon weirdWeapon("");
    HumanA ghost("", weirdWeapon);

    std::cout << C_YELLOW << "-> Nameless HumanA with a nameless weapon:" << C_RESET << std::endl;
    ghost.attack();
  }

  std::cout << C_BOLD << C_GREEN << "\n=== ALL TESTS COMPLETED SUCCESSFULLY ===\n" << C_RESET << std::endl;

  return 0;
}
