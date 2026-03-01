/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 17:54:24 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:12:46 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE02_EX00_FIXED_HPP_
#define CPP_MODULE02_EX00_FIXED_HPP_

#include <iostream>

class Fixed {
 private:
  int _fixedPointValue;
  static const int _fractionalBits = 8;

 public:
  Fixed();                    // Default constructor
  Fixed(const Fixed& other);  // Copy constructor
  Fixed& operator=(
      const Fixed& other);  // Copy assignment operator
  ~Fixed();                 // Destructor

  int getRawBits(void) const;
  void setRawBits(int const raw);
};

#endif  // CPP_MODULE02_EX00_FIXED_HPP_
