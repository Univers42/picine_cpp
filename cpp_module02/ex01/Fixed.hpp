/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 18:08:24 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:47:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE02_EX01_FIXED_HPP_
#define CPP_MODULE02_EX01_FIXED_HPP_
#include <cmath>
#include <iostream>

class Fixed {
 private:
  int _fixedPointValue;
  static const int _fractionalBits = 8;

 public:
  Fixed();
  Fixed(const Fixed& other);
  Fixed& operator=(const Fixed& other);
  ~Fixed();
  explicit Fixed(const int n);
  explicit Fixed(const float f);
  int getRawBits(void) const;
  void setRawBits(int const raw);
  float toFloat(void) const;
  int toInt(void) const;
};

std::ostream& operator<<(std::ostream& o, Fixed const& i);

#endif  // CPP_MODULE02_EX01_FIXED_HPP_
