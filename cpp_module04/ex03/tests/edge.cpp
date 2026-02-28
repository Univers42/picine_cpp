/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edge.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:21:59 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 00:22:00 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "AMateria.hpp"
#include "Character.hpp"
#include "Cure.hpp"
#include "ICharacter.hpp"
#include "IMateriaSource.hpp"
#include "Ice.hpp"
#include "MateriaSource.hpp"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

int main() {
  std::cout << CYAN
            << "\n========================================================"
            << RESET << std::endl;
  std::cout << "--- PHASE 1: Null Pointers & Unknown Types ---" << std::endl;
  std::cout << CYAN
            << "========================================================\n"
            << RESET << std::endl;

  IMateriaSource* src = new MateriaSource();
  ICharacter* cloud = new Character("Cloud");

  std::cout << YELLOW << "[Testing] Learning NULL Materia..." << RESET
            << std::endl;
  src->learnMateria(NULL);  // Should do nothing, no crash

  std::cout << YELLOW << "[Testing] Creating Unknown Materia..." << RESET
            << std::endl;
  AMateria* unknown = src->createMateria("fire");  // Should return NULL
  if (unknown == NULL)
    std::cout << GREEN << "✅ Correctly returned NULL for 'fire'" << RESET
              << std::endl;

  std::cout << YELLOW << "[Testing] Equipping NULL Materia..." << RESET
            << std::endl;
  cloud->equip(NULL);  // Should do nothing, no crash

  std::cout << CYAN
            << "\n========================================================"
            << RESET << std::endl;
  std::cout << "--- PHASE 2: Out of Bounds & Empty Slots ---" << std::endl;
  std::cout << CYAN
            << "========================================================\n"
            << RESET << std::endl;

  ICharacter* sephiroth = new Character("Sephiroth");

  std::cout << YELLOW << "[Testing] Using empty slots..." << RESET << std::endl;
  cloud->use(0, *sephiroth);  // Should do nothing
  cloud->use(3, *sephiroth);  // Should do nothing

  std::cout << YELLOW << "[Testing] Out of bounds index (-1 and 5)..." << RESET
            << std::endl;
  cloud->use(-1, *sephiroth);  // Should do nothing
  cloud->use(5, *sephiroth);   // Should do nothing
  cloud->unequip(-1);          // Should do nothing
  cloud->unequip(5);           // Should do nothing

  std::cout << GREEN
            << "✅ Survived all out-of-bounds attacks without crashing!"
            << RESET << std::endl;

  std::cout << CYAN
            << "\n========================================================"
            << RESET << std::endl;
  std::cout << "--- PHASE 3: Inventory Overflow ---" << std::endl;
  std::cout << CYAN
            << "========================================================\n"
            << RESET << std::endl;

  src->learnMateria(new Ice());
  src->learnMateria(new Cure());

  std::cout << YELLOW << "[Testing] Equipping 5 items (Max is 4)..." << RESET
            << std::endl;
  cloud->equip(src->createMateria("ice"));   // Slot 0
  cloud->equip(src->createMateria("cure"));  // Slot 1
  cloud->equip(src->createMateria("ice"));   // Slot 2
  cloud->equip(src->createMateria("cure"));  // Slot 3

  // The 5th item will be rejected. We must track the pointer to avoid a leak!
  AMateria* rejectedItem = src->createMateria("ice");
  cloud->equip(rejectedItem);  // Should be rejected safely

  std::cout << "Cloud using slot 3 (Should be Cure): ";
  cloud->use(3, *sephiroth);
  std::cout << "Cloud using slot 4 (Should do nothing, rejected): ";
  cloud->use(4, *sephiroth);

  delete rejectedItem;  // Manually clean up the item that couldn't fit

  std::cout << CYAN
            << "\n========================================================"
            << RESET << std::endl;
  std::cout << "--- PHASE 4: Aggressive Overwrite (Deep Copy) ---" << std::endl;
  std::cout << CYAN
            << "========================================================\n"
            << RESET << std::endl;

  Character* tifa = new Character("Tifa");
  tifa->equip(src->createMateria("cure"));
  tifa->equip(src->createMateria("cure"));

  std::cout << "Tifa before assignment uses slot 0: ";
  tifa->use(0, *sephiroth);

  std::cout << YELLOW << "[Testing] Assigning fully equipped Cloud to Tifa..."
            << RESET << std::endl;
  // This must DELETE Tifa's two 'cure' materias, and deeply clone Cloud's 4
  // materias!
  *tifa = *(Character*)cloud;

  std::cout << "Tifa after assignment uses slot 0 (Should now be Ice): ";
  tifa->use(0, *sephiroth);

  std::cout << "\n[ Cleaning Up ]" << std::endl;
  delete src;
  delete cloud;
  delete sephiroth;
  delete tifa;

  std::cout << GREEN << "\n✅ ALL FUZZ TESTS COMPLETED SAFELY!" << RESET
            << std::endl;
  return 0;
}
