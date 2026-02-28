/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:49:24 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 00:06:05 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "AAnimal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

int main() {
  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 1. Testing Abstract Class Instantiation ---" << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  std::cout << "If you uncomment the line below, the code WILL NOT compile:"
            << std::endl;
  // AAnimal* impossible = new AAnimal();
  // Error: allocating an object of abstract class type 'AAnimal'

  std::cout << "AAnimal is safely abstract! Only Dogs and Cats can be built."
            << std::endl;

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 2. Verifying Polymorphism & Deep Copies ---" << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  const int numAnimals = 4;
  AAnimal* animals[numAnimals];

  std::cout << "[ Allocating Animals ]" << std::endl;
  for (int i = 0; i < numAnimals; i++) {
    if (i < numAnimals / 2) {
      animals[i] = new Dog();
    } else {
      animals[i] = new Cat();
    }
  }

  std::cout << "\n[ Checking Sounds ]" << std::endl;
  for (int i = 0; i < numAnimals; i++) {
    std::cout << animals[i]->getType() << " says: ";
    animals[i]->makeSound();
  }

  std::cout << "\n[ Deleting Animals (Checking Leaks) ]" << std::endl;
  for (int i = 0; i < numAnimals; i++) {
    delete animals[i];
  }

  return 0;
}
