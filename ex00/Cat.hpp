/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cat.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:48:40 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:13:36 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE04_EX00_CAT_HPP_
#define CPP_MODULE04_EX00_CAT_HPP_

#include "Animal.hpp"

/**
 * @class Cat
 * @brief A specific type of Animal that demonstraes polymorphic behavior
 */
class Cat : public Animal {
 public:
  Cat();
  Cat(const Cat& src);
  Cat& operator=(const Cat& rhs);
  virtual ~Cat();
  virtual void makeSound() const;
};

#endif  // CPP_MODULE04_EX00_CAT_HPP_
