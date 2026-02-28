/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:49:24 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 23:51:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "Animal.hpp"
#include "Cat.hpp"
#include "Dog.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int main() {
  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 1. Mandatory Subject Tests (Proper Polymorphism) ---"
            << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  const Animal* meta = new Animal();
  const Animal* j = new Dog();
  const Animal* i = new Cat();

  std::cout << "\n[ Types ]" << std::endl;
  std::cout << "j is a: " << j->getType() << " " << std::endl;
  std::cout << "i is a: " << i->getType() << " " << std::endl;

  std::cout << "\n[ Sounds (Virtual Dispatch) ]" << std::endl;
  i->makeSound();  // Will output the cat sound!
  j->makeSound();
  meta->makeSound();

  std::cout << "\n[ Destruction (Virtual Destructors) ]" << std::endl;
  delete meta;
  delete j;
  delete i;

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 2. 'Wrong' Tests (Without Virtual Keywords) ---"
            << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  const WrongAnimal* wrongMeta = new WrongAnimal();
  const WrongAnimal* wrongI = new WrongCat();

  std::cout << "\n[ Types ]" << std::endl;
  std::cout << "wrongI is a: " << wrongI->getType() << " " << std::endl;

  std::cout << "\n[ Sounds (Static Dispatch failure) ]" << std::endl;
  // Because makeSound() is NOT virtual in WrongAnimal, the compiler looks at
  // the pointer type (WrongAnimal*) and calls the base class function!
  wrongI->makeSound();
  wrongMeta->makeSound();

  std::cout << "\n[ Destruction ]" << std::endl;
  delete wrongMeta;
  delete wrongI;

  return 0;
}
