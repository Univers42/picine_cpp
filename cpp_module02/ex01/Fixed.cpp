/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 18:08:29 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 18:39:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Fixed.hpp"

Fixed::Fixed() : _fixedPointValue(0) {
  std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const int n) : _fixedPointValue(n << _fractionalBits) {
  std::cout << "Int constructor called" << std::endl;
}

Fixed::Fixed(const float f)
    : _fixedPointValue(roundf(f * (1 << _fractionalBits))) {
  std::cout << "Float constructor called" << std::endl;
}

Fixed::Fixed(const Fixed &other) {
  std::cout << "Copy constructor called" << std::endl;
  *this = other;
}

Fixed &Fixed::operator=(const Fixed &other) {
  std::cout << "Copy assignment operator called" << std::endl;
  if (this != &other) this->_fixedPointValue = other.getRawBits();
  return *this;
}

Fixed::~Fixed() { std::cout << "Destructor called" << std::endl; }

int Fixed::getRawBits(void) const { return this->_fixedPointValue; }

void Fixed::setRawBits(int const raw) { this->_fixedPointValue = raw; }

float Fixed::toFloat(void) const {
  return (float)this->_fixedPointValue / (1 << _fractionalBits);
}

// Standard right shift is usually okay, but casting to float
// first ensures the truncation behaves as expected for negatives
int Fixed::toInt(void) const { return static_cast<int>(this->toFloat()); }

std::ostream &operator<<(std::ostream &o, Fixed const &i) {
  o << i.toFloat();
  return o;
}
