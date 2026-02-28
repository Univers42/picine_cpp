/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 17:54:34 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 18:06:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

// Colors for professional presentation
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define DIM "\033[2m"

Fixed::Fixed() : _fixedPointValue(0) {
  std::cout << GREEN << "Default constructor called" << RESET << std::endl;
}

Fixed::Fixed(const Fixed &other) {
  std::cout << YELLOW << "Copy constructor called" << RESET << std::endl;
  *this = other;
}

Fixed &Fixed::operator=(const Fixed &other) {
  std::cout << CYAN << "Copy assignment operator called" << RESET << std::endl;
  if (this != &other) this->_fixedPointValue = other.getRawBits();
  return *this;
}

Fixed::~Fixed() {
  std::cout << RED << "Destructor called" << RESET << std::endl;
}

int Fixed::getRawBits(void) const {
  std::cout << DIM << "  getRawBits member function called" << RESET
            << std::endl;
  return this->_fixedPointValue;
}

void Fixed::setRawBits(int const raw) { this->_fixedPointValue = raw; }
