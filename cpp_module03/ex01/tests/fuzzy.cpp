/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fuzzy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:23:57 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 23:25:08 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "ScavTrap.hpp"

// Utility for clean, automated reporting
void assertTest(const std::string& name, bool condition) {
  if (condition) {
    std::cout << GREEN << "[PASS] " << RESET << name << std::endl;
  } else {
    std::cout << RED << "[FAIL] " << RESET << name << std::endl;
  }
}

int main() {
  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- SCAVTRAP AUTOMATED EDGE-CASE FUZZER ---" << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  // 1. Initialization Test
  std::cout << "\n[ Phase 1: Stat Initialization ]" << std::endl;
  ScavTrap st("FuzzerBot");
  assertTest("Starting HP is 100", st.getHitPoints() == 100);
  assertTest("Starting EP is 50", st.getEnergyPoints() == 50);
  assertTest("Starting AD is 20", st.getAttackDamage() == 20);

  // 2. Exhaustion Test (EP Drain)
  std::cout << "\n[ Phase 2: Extreme EP Drain ]" << std::endl;
  for (int i = 0; i < 50; ++i) {
    st.attack("Dummy");  // Should work 50 times
  }
  assertTest("EP reached exactly 0", st.getEnergyPoints() == 0);

  // This 51st action should be blocked and NOT decrease EP to underflow
  st.attack("Dummy");
  st.beRepaired(10);
  assertTest("EP did not underflow after exhaustion",
             st.getEnergyPoints() == 0);

  // 3. Lethal Damage & Underflow Test
  std::cout << "\n[ Phase 3: Lethal Damage & Underflow ]" << std::endl;
  ScavTrap victim("PoorBot");
  victim.takeDamage(500);  // Massive overkill damage
  assertTest("HP stopped exactly at 0 (No Underflow)",
             victim.getHitPoints() == 0);

  // 4. Dead State Action Test
  std::cout << "\n[ Phase 4: Actions While Dead ]" << std::endl;
  victim.attack("Anyone");
  victim.beRepaired(50);
  victim.guardGate();
  assertTest("HP remained 0 after attempted repair",
             victim.getHitPoints() == 0);

  // 5. Orthodox Canonical Form Test
  std::cout << "\n[ Phase 5: Deep Copy Validation ]" << std::endl;
  ScavTrap original("Prime");
  original.takeDamage(10);  // HP = 90
  original.attack("X");     // EP = 49

  ScavTrap clone(original);  // Copy Constructor
  ScavTrap assigned("Temp");
  assigned = original;  // Assignment Operator

  assertTest("Clone inherited exact HP (90)", clone.getHitPoints() == 90);
  assertTest("Assigned inherited exact EP (49)",
             assigned.getEnergyPoints() == 49);
  assertTest("Clone retained ScavTrap AD (20)", clone.getAttackDamage() == 20);

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- END OF TESTS (Destructors will now fire) ---" << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  return 0;
}
