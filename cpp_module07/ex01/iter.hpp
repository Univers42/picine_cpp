/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iter.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:37:11 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 11:47:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE07_EX01_ITER_HPP_
#define CPP_MODULE07_EX01_ITER_HPP_

#include <cstddef>  // required for size_t

/**
 * @brief Iterates over an array and applies a function to each element
 * @tparam the type of the array elements (deduced automatically)
 * @param F the type of the callable function or function pointer.
 * @param array the address of the array to iterate over.
 * @param length the length fo the array (passed as const).
 * @param f the functio to execute on each element.
 */
template <typename T, typename F>
void iter(T* array, const size_t length, F f) {
  for (size_t i = 0; i < length; ++i) f(array[i]);
}

#endif  // CPP_MODULE07_EX01_ITER_HPP_
