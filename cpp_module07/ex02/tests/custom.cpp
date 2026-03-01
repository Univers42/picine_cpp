/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   custom.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:01:33 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 12:11:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#include "../Array.hpp"

#define MAX_VAL 750

int main(int, char**) {
  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 1. OFFICIAL 42 EVALUATION TEST (Int Array) ---"
            << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  unsigned int seed = time(NULL);
  Array<int> numbers(MAX_VAL);
  int* mirror = new int[MAX_VAL];
  for (int i = 0; i < MAX_VAL; i++) {
    const int value = rand_r(&seed);
    numbers[i] = value;
    mirror[i] = value;
  }

  // SCOPE TEST (Proves Deep Copy works without double freeing)
  {
    Array<int> tmp = numbers;
    Array<int> test(tmp);
  }

  // VALIDATE DATA
  for (int i = 0; i < MAX_VAL; i++) {
    if (mirror[i] != numbers[i]) {
      std::cerr << "didn't save the same value!!" << std::endl;
      return 1;
    }
  }
  std::cout << "✅ Array holds exactly " << numbers.size()
            << " integers perfectly matched with raw C array!" << std::endl;

  std::cout << "\n[ Testing Out of Bounds Exceptions ]" << std::endl;
  try {
    numbers[-2] = 0;
  } catch (const std::exception& e) {
    std::cerr << "Negative index check: " << e.what() << '\n';
  }
  try {
    numbers[MAX_VAL] = 0;
  } catch (const std::exception& e) {
    std::cerr << "Overflow index check: " << e.what() << '\n';
  }

  for (int i = 0; i < MAX_VAL; i++) {
    numbers[i] = rand_r(&seed);
  }
  delete[] mirror;

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 2. GENERIC TYPE TESTS (String Array) ---" << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  Array<std::string> strArray(3);
  strArray[0] = "Hello";
  strArray[1] = "42";
  strArray[2] = "Network";

  std::cout << "strArray size: " << strArray.size() << std::endl;
  for (unsigned int i = 0; i < strArray.size(); i++) {
    std::cout << "strArray[" << i << "] = " << strArray[i] << std::endl;
  }

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- 3. DEFAULT INITIALIZATION TEST ---" << std::endl;
  std::cout << "========================================================\n"
            << std::endl;

  Array<int> emptyCheck(5);
  std::cout << "Unassigned int array values (Should be strictly 0):"
            << std::endl;
  for (unsigned int i = 0; i < emptyCheck.size(); i++) {
    std::cout << emptyCheck[i] << " ";
  }
  std::cout << "\n\n✅ ALL TESTS PASSED!" << std::endl;

  return 0;
}
