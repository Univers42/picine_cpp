/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:48:29 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:24:01 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"
#include <iostream>

Dog::Dog() : Animal() {
  this->type = "Dog";  // Set type to Dog [cite: 162]
  std::cout << GREEN << "Dog default constructor called" << RESET << std::endl;
}

Dog::Dog(const Dog& src) : Animal(src) {
  std::cout << YELLOW << "Dog copy constructor called" << RESET << std::endl;
}

Dog& Dog::operator=(const Dog& rhs) {
  std::cout << BLUE << "Dog assignment operator called" << RESET << std::endl;
  if (this != &rhs) {
    Animal::operator=(rhs);
    this->type = rhs.type;
  }
  return *this;
}

Dog::~Dog() {
  std::cout << RED << "Dog destructor called" << RESET << std::endl;
}

void Dog::makeSound() const { std::cout << "Woof! Woof! Bark!" << std::endl; }
