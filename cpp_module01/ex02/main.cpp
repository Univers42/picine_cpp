/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 06:26:07 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 06:36:30 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

/**
 * PURPOSE: Demontrate the use of pointers, variables, references in C++
 *
 * COMMON: each one of them can access values in memory
 *
 * USE_CASES:
 * - variable:
 *   - actually own the storage for the value.
 *   - We access teh value directly
 *   - Cheap and clear for small values or when we want ownership
 * - pointer:
 *   - is a variable taht store an address
 *   - can be null, can be reassigned to another address, we must dereference
 *     to access the value (*ptr)
 *   - useful when we need optional ownership, reseating, or pointer arithmetic
 *     or interact with C apis
 *  - Reference (std::string&)
 *   - is an alias for an existing object (no reseating, cannot be null)
 *   - syntax looks like the original object (use it like greet), no explicit
 *     dereference
 *   - ideal for function parameters/returns when we want to avoid copies and
 *     don't need nullability or reseating
 */
int main(void)
{
  std::string greet = "Hi this is brain";
  std::string* stringPTR = &greet;
  std::string& stringREF = greet;
  std::cout << "Address of greet: " << &greet << std::endl;
  std::cout << "Address held by stringPTR: " << stringPTR << std::endl;
  std::cout << "Address held by stringREF: " << &stringREF << std::endl;
  std::cout << "Value of greet: " << greet << std::endl;
  std::cout << "Value pointed to by stringPTR: " << *stringPTR << std::endl;
  std::cout << "Value pointed to by stringREF: " << stringREF << std::endl;
}
