/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:44:54 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:54:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE01_EX06_HARL_HPP_
#define CPP_MODULE01_EX06_HARL_HPP_

#include <iostream>
#include <string>

class Harl {
 public:
  Harl();
  ~Harl();
  void complain(std::string level);

 private:
  void debug(void);
  void info(void);
  void warning(void);
  void error(void);
};

#endif  // CPP_MODULE01_EX06_HARL_HPP_
