/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edge2.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:22:53 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 00:24:28 by dlesieur         ###   ########.fr       */
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
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

int main() {
  std::cout << BOLD << CYAN
            << "\n========================================================"
            << RESET << std::endl;
  std::cout << BOLD
            << "--- CONTROL TEST 1: MateriaSource Capacity & Unknowns ---"
            << RESET << std::endl;
  std::cout << BOLD << CYAN
            << "========================================================\n"
            << RESET << std::endl;

  IMateriaSource* src = new MateriaSource();

  std::cout << YELLOW << "[ Action ] Learning exactly 4 Materias..." << RESET
            << std::endl;
  src->learnMateria(new Ice());   // Slot 0
  src->learnMateria(new Cure());  // Slot 1
  src->learnMateria(new Ice());   // Slot 2
  src->learnMateria(new Cure());  // Slot 3

  std::cout << YELLOW
            << "[ Action ] Attempting to learn a 5th Materia (Overflow)..."
            << RESET << std::endl;
  // If your MateriaSource is coded correctly, it should reject this AND delete
  // it to prevent leaks!
  src->learnMateria(new Ice());
  std::cout << GREEN << "-> 5th Materia safely handled by MateriaSource."
            << RESET << std::endl;

  std::cout << YELLOW
            << "[ Action ] Creating an unknown Materia ('lightning')..."
            << RESET << std::endl;
  AMateria* invalid = src->createMateria("lightning");
  if (invalid == NULL) {
    std::cout << GREEN << "-> Correctly returned NULL for unknown type."
              << RESET << std::endl;
  }

  std::cout << BOLD << CYAN
            << "\n========================================================"
            << RESET << std::endl;
  std::cout << BOLD
            << "--- CONTROL TEST 2: Inventory Holes & Floor Management ---"
            << RESET << std::endl;
  std::cout << BOLD << CYAN
            << "========================================================\n"
            << RESET << std::endl;

  ICharacter* zack = new Character("Zack");
  ICharacter* dummy = new Character("Dummy");

  std::cout << YELLOW << "[ Action ] Filling Zack's inventory (4 slots)..."
            << RESET << std::endl;

  // We explicitly save this pointer so we can safely unequip it later without
  // leaking memory!
  AMateria* savedCure = src->createMateria("cure");

  zack->equip(src->createMateria("ice"));   // 0
  zack->equip(savedCure);                   // 1 (Saved pointer)
  zack->equip(src->createMateria("ice"));   // 2
  zack->equip(src->createMateria("cure"));  // 3

  std::cout << YELLOW
            << "[ Action ] Attempting to equip a 5th item (Inventory Full)..."
            << RESET << std::endl;
  AMateria* overflowItem = src->createMateria("ice");
  zack->equip(overflowItem);  // Should reject

  std::cout << YELLOW << "[ Action ] Unequipping Slot 1 (Creating a hole)..."
            << RESET << std::endl;
  zack->unequip(1);  // Hole created at index 1, savedCure is now "on the floor"

  std::cout << YELLOW
            << "[ Action ] Equipping the previously rejected 5th item..."
            << RESET << std::endl;
  zack->equip(overflowItem);  // Should seamlessly slide into Slot 1!

  std::cout << "Zack using Slot 1 (Should be ICE now, not cure): ";
  zack->use(1, *dummy);

  std::cout << BOLD << CYAN
            << "\n========================================================"
            << RESET << std::endl;
  std::cout << BOLD << "--- CONTROL TEST 3: Asymmetrical Deep Assignment ---"
            << RESET << std::endl;
  std::cout << BOLD << CYAN
            << "========================================================\n"
            << RESET << std::endl;

  Character* a = new Character("Character_A");
  a->equip(src->createMateria("cure"));  // A only has 1 item

  Character* b = new Character("Character_B");
  b->equip(src->createMateria("ice"));
  b->equip(src->createMateria("ice"));
  b->equip(src->createMateria("ice"));  // B has 3 items

  std::cout << YELLOW << "[ Action ] Assigning B (3 items) to A (1 item)..."
            << RESET << std::endl;
  *a = *b;  // A's Cure MUST be deleted, and B's 3 Ices must be deeply copied.

  std::cout << "A using Slot 0 (Should be ICE): ";
  a->use(0, *dummy);
  std::cout << "A using Slot 2 (Should be ICE): ";
  a->use(2, *dummy);
  std::cout << "A using Slot 3 (Should be empty, do nothing): ";
  a->use(3, *dummy);

  std::cout << YELLOW << "[ Action ] Deleting B to prove A made a Deep Copy..."
            << RESET << std::endl;
  delete b;

  std::cout << "A using Slot 0 after B is destroyed: ";
  a->use(0, *dummy);

  std::cout << BOLD << "\n[ Final Cleanup to satisfy Valgrind ]" << RESET
            << std::endl;
  delete a;
  delete zack;
  delete dummy;
  delete src;

  // Clean up the item we safely left on the floor!
  // If unequip() had illegally deleted it, this line would crash the program.
  delete savedCure;

  std::cout << GREEN << "\n✅ ALL CONTROL TESTS COMPLETED!" << RESET
            << std::endl;
  return 0;
}
