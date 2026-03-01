/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easyfind.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:14:44 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 12:22:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE08_EX00_EASYFIND_HPP_
#define CPP_MODULE08_EX00_EASYFIND_HPP_

#include <algorithm>
#include <exception>

/**
 * @brief Exception thrown when the target value is not found in the container.
 */
class NotFoundException : public std::exception {
 public:
  virtual const char* what() const throw() {
    return "Error: Value not found in container.";
  }
};

/**
 * @brief Finds the first occurrence of an integer in a mutable container.
 * Uses std::find for O(N) time complexity and O(1) space complexity.
 * * @tparam T The type of the container.
 * @param container The container to search in.
 * @param value The integer value to find.
 * @return T::iterator An iterator pointing to the found element.
 * @throws NotFoundException if the value is not found.
 */
template <typename T>
typename T::iterator easyfind(T& container, int value) {
  typename T::iterator it =
      std::find(container.begin(), container.end(), value);
  if (it == container.end()) throw(NotFoundException());
  return (it);
}

/**
 * @brief Finds the first occurrence of an integer in a constant container.
 * * @tparam T The type of the container.
 * @param container The const container to search in.
 * @param value The integer value to find.
 * @return T::const_iterator A const_iterator pointing to the found element.
 * @throws NotFoundException if the value is not found.
 */
template <typename T>
typename T::const_iterator easyfind(const T& container, int value) {
  typename T::const_iterator it =
      std::find(container.begin(), container.end(), value);
  if (it == container.end()) throw(NotFoundException());
  return (it);
}

#endif  // CPP_MODULE08_EX00_EASYFIND_HPP_
