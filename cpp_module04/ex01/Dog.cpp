/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:48:29 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 00:01:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Dog.hpp"
#include <iostream>

Dog::Dog() : Animal() {
  this->type = "Dog";
  this->_brain = new Brain();  // Allocate new brain
  std::cout << GREEN << "Dog default constructor called" << RESET << std::endl;
}

Dog::Dog(const Dog& src) : Animal(src) {
  this->_brain = new Brain(*src._brain);  // Deep Copy [cite: 201]
  std::cout << YELLOW << "Dog deep-copy constructor called" << RESET
            << std::endl;
}

Dog& Dog::operator=(const Dog& rhs) {
  std::cout << BLUE << "Dog deep-copy assignment operator called" << RESET
            << std::endl;
  if (this != &rhs) {
    Animal::operator=(rhs);
    delete this->_brain;                    // Prevent memory leak of old brain
    this->_brain = new Brain(*rhs._brain);  // Deep Copy [cite: 201]
  }
  return *this;
}

Dog::~Dog() {
  delete this->_brain;  // Prevent memory leaks
  std::cout << RED << "Dog destructor called" << RESET << std::endl;
}

void Dog::makeSound() const { std::cout << "Woof! Woof! Bark!" << std::endl; }

Brain* Dog::getBrain() const { return this->_brain; }
