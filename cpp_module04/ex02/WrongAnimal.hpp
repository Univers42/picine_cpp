/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WrongAnimal.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 23:48:58 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 00:03:59 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE04_EX02_WRONGANIMAL_HPP_
#define CPP_MODULE04_EX02_WRONGANIMAL_HPP_

#include <iostream>
#include <string>

class WrongAnimal {
 protected:
  std::string type;

 public:
  WrongAnimal();
  WrongAnimal(const WrongAnimal& src);
  WrongAnimal& operator=(const WrongAnimal& rhs);
  virtual ~WrongAnimal();

  std::string getType() const;

  // DELIBERATELY MISSING 'virtual' KEYWORD TO BREAK POLYMORPHISM [cite: 181]
  void makeSound() const;
};

#endif  // CPP_MODULE04_EX02_WRONGANIMAL_HPP_
