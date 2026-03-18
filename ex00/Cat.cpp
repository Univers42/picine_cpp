/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:48:46 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/06 15:12:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cat.hpp"
#include <iostream>

Cat::Cat() : Animal() {
  this->type = "Cat";
  std::cout << GREEN << "Cat default constructor called" << RESET << std::endl;
}

Cat::Cat(const Cat& src) : Animal(src) {
  std::cout << YELLOW << "Cat copy constructor called" << RESET << std::endl;
}

Cat& Cat::operator=(const Cat& rhs) {
  std::cout << BLUE << "Cat assignment operator called" << RESET << std::endl;
  if (this != &rhs) {
    Animal::operator=(rhs);
    this->type = rhs.type;
  }
  return *this;
}

Cat::~Cat() {
  std::cout << RED << "Cat destructor called" << RESET << std::endl;
}

void Cat::makeSound() const {
  std::cout << "Meow! Purrrrr..."
            << std::endl;  // Appropriate sound
}
