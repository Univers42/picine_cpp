/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Dog.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:48:13 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:13:50 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE04_EX00_DOG_HPP_
#define CPP_MODULE04_EX00_DOG_HPP_

#include "Animal.hpp"

class Dog : public Animal {
 public:
  Dog();
  Dog(const Dog& src);
  Dog& operator=(const Dog& rhs);
  virtual ~Dog();

  virtual void makeSound() const;
};

#endif  // CPP_MODULE04_EX00_DOG_HPP_
