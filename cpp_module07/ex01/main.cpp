/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:37:09 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 11:48:39 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "iter.hpp"

/**
 * To solve this exercice, we need to build a generic algorithm that
 * traverses an array and applies a cb fn
 * From a DSA insight, traversing an array of N elements takes O(N) time
 * complexity. To ensure optimal O(1) space complexity and avoid severe
 * performance bottlenecks,we  must guarantee that the elem are passed to the
 * cb function by reference T& or const T&, rather than by value, so no
 * unnecessary memory allocation during hte iteration.
 *
 * The subject explicitly challenges us to handle both `const` and `non-const`
 * arrays cleanly. By using a double-template approach (`typename T`, `typename
 * F`), The C++ compiler will mathematically deduce the exact constness of
 * the array and the exact signature of the function automaticalñy
 */

/* --- 1. Standard function (Non-Const) --- */
void incrementElement(int& n) { n++; }

/* --- 2. Standard function (Const) --- */
void printInt(const int& n) { std::cout << n << " "; }

/* --- 3. Function Template --- */
template <typename T>
void printElement(const T& elem) {
  std::cout << elem << " ";
}

__attribute__((weak)) int main(void) {
  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- TEST 1: Mutable Array (Non-Const Operations) ---"
            << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  int intArray[] = {1, 2, 3, 4, 5};
  const size_t intLen = sizeof(intArray) / sizeof(intArray[0]);

  std::cout << "Original intArray:   ";
  ::iter(intArray, intLen, printInt);
  std::cout << std::endl;

  std::cout << "[ Action ] Incrementing all elements..." << std::endl;
  // Passes a non-const reference function to modify the array in O(N) time
  ::iter(intArray, intLen, incrementElement);

  std::cout << "Modified intArray:   ";
  ::iter(intArray, intLen, printInt);
  std::cout << std::endl;

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- TEST 2: Constant Array & Instantiated Template ---"
            << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  const std::string stringArray[] = {"Hello", "42", "Madrid", "C++98"};
  const size_t stringLen = sizeof(stringArray) / sizeof(stringArray[0]);

  std::cout << "stringArray: ";
  // The subject explicitly asks to test passing an instantiated function
  // template!
  ::iter(stringArray, stringLen, printElement<std::string>);
  std::cout << std::endl << std::endl;

  return 0;
}
