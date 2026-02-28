/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 00:12:11 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 00:21:28 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "AMateria.hpp"
#include "Character.hpp"
#include "Cure.hpp"
#include "ICharacter.hpp"
#include "IMateriaSource.hpp"
#include "Ice.hpp"
#include "MateriaSource.hpp"

__attribute__((weak)) int main() {
  std::cout << "--- 1. Mandatory Subject Test ---" << std::endl;
  IMateriaSource* src = new MateriaSource();
  src->learnMateria(new Ice());
  src->learnMateria(new Cure());

  ICharacter* me = new Character("me");

  AMateria* tmp;
  tmp = src->createMateria("ice");
  me->equip(tmp);
  tmp = src->createMateria("cure");
  me->equip(tmp);

  ICharacter* bob = new Character("bob");
  me->use(0, *bob);
  me->use(1, *bob);

  std::cout << "\n--- 2. Deep Copy Test ---" << std::endl;
  Character* original = new Character("Original");
  original->equip(src->createMateria("ice"));

  Character* clone = new Character(*original);  // Deep copy construction
  std::cout << "Original uses slot 0: ";
  original->use(0, *bob);
  std::cout << "Clone uses slot 0:    ";
  clone->use(0, *bob);

  delete original;  // Delete original, clone should still have its own Ice!
  std::cout << "Clone uses slot 0 after Original is deleted: ";
  clone->use(0, *bob);

  std::cout << "\n--- 3. Unequip & Floor Leak Test ---" << std::endl;
  AMateria* floorItem = src->createMateria("cure");
  clone->equip(floorItem);

  // Subject states: "Save the addresses before calling unequip() ...
  // you have to avoid memory leaks."
  clone->unequip(1);  // Item dropped on the floor

  clone->use(1, *bob);  // Should do nothing, slot is empty

  // Cleanup
  delete floorItem;  // Picked up from the floor and destroyed
  delete clone;
  delete bob;
  delete me;
  delete src;

  return 0;
}
