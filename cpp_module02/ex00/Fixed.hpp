/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Fixed.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 17:54:24 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 18:06:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>

class Fixed {
 private:
  int _fixedPointValue;
  static const int _fractionalBits = 8;

 public:
  Fixed();                    // Default constructor [cite: 75, 149]
  Fixed(const Fixed &other);  // Copy constructor [cite: 76, 150]
  Fixed &operator=(
      const Fixed &other);  // Copy assignment operator [cite: 77, 151]
  ~Fixed();                 // Destructor [cite: 78, 152]

  int getRawBits(void) const;      // [cite: 153]
  void setRawBits(int const raw);  //
};

#endif
