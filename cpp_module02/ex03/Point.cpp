/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 21:44:31 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 21:53:07 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* Point.cpp                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/02/28 21:44:31 by dlesieur          #+#    #+#             */
/* Updated: 2026/02/28 21:48:07 by dlesieur         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "Point.hpp"

// Default constructor
Point::Point() : _x(0), _y(0) {}

// Missing parameterized constructor added here!
Point::Point(const float x, const float y) : _x(x), _y(y) {}

// Copy constructor
Point::Point(const Point &other) : _x(other._x), _y(other._y) {}

/**
 * TRAP BYPASS: _x and _y are const, so standard assignment fails
 * We cast away constness to satisfy the Orthodox Form requirements
 */
Point &Point::operator=(const Point &other) {
  if (this != &other) {
    (Fixed &)this->_x = other.getX();
    (Fixed &)this->_y = other.getY();
  }
  return (*this);
}

// Destructor
Point::~Point() {}

// Getters
Fixed const &Point::getX(void) const { return (this->_x); }
Fixed const &Point::getY(void) const { return (this->_y); }
