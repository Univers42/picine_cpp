/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:49:24 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 00:00:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"

int main() {
  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 1. Mandatory Array Test (Allocation & Deletion) ---"
            << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  const int numAnimals = 4;
  Animal* animals[numAnimals];

  // Fill half with Dogs, half with Cats [cite: 197]
  std::cout << "[ Allocating Animals ]" << std::endl;
  for (int i = 0; i < numAnimals; i++) {
    if (i < numAnimals / 2) {
      animals[i] = new Dog();
    } else {
      animals[i] = new Cat();
    }
  }

  // Loop over array and delete every animal [cite: 198, 199]
  std::cout << "\n[ Deleting Animals ]" << std::endl;
  for (int i = 0; i < numAnimals; i++) {
    delete animals[i];
  }

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 2. Deep Copy Proof Test ---" << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  std::cout << "[ Creating Original Dog ]" << std::endl;
  Dog* originalDog = new Dog();
  originalDog->getBrain()->ideas[0] = "I want to chase the postman!";

  std::cout << "\n[ Creating Copy Dog via Copy Constructor ]" << std::endl;
  Dog* copyDog = new Dog(*originalDog);

  std::cout << "\n[ Comparing Brain Addresses ]" << std::endl;
  std::cout << "Original Brain PTR: " << originalDog->getBrain() << std::endl;
  std::cout << "Copy Brain PTR:     " << copyDog->getBrain() << std::endl;

  std::cout << "\n[ Modifying Original Dog's Idea ]" << std::endl;
  originalDog->getBrain()->ideas[0] = "Actually, I want a nap.";

  std::cout << "Original Idea: " << originalDog->getBrain()->ideas[0]
            << std::endl;
  std::cout << "Copy Idea:     " << copyDog->getBrain()->ideas[0] << std::endl;

  if (originalDog->getBrain()->ideas[0] != copyDog->getBrain()->ideas[0]) {
    std::cout << GREEN << "\n✅ DEEP COPY SUCCESSFUL! Ideas are independent."
              << RESET << std::endl;
  } else {
    std::cout << RED << "\n❌ SHALLOW COPY DETECTED! Both changed." << RESET
              << std::endl;
  }

  std::cout << "\n[ Cleaning Up ]" << std::endl;
  delete originalDog;
  delete copyDog;

  return 0;
}
