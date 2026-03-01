/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fuzzy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:59:21 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 12:59:35 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <stack>
#include <vector>

#include "../MutantStack.hpp"

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"

int main() {
  std::cout << MAGENTA << "========================================" << RESET
            << std::endl;
  std::cout << MAGENTA << "    MUTANTSTACK HARDCORE FUZZ TESTER    " << RESET
            << std::endl;
  std::cout << MAGENTA << "========================================" << RESET
            << std::endl;

  std::cout << "\n"
            << CYAN << "--- TEST 1: Const Iterators ---" << RESET << std::endl;
  MutantStack<int> mstack1;
  mstack1.push(10);
  mstack1.push(20);
  mstack1.push(30);

  // Deep copying to a strictly const stack
  const MutantStack<int> const_mstack(mstack1);
  std::cout << "Iterating const stack (Expected: 10 20 30):" << std::endl;

  for (MutantStack<int>::const_iterator it = const_mstack.begin();
       it != const_mstack.end(); ++it) {
    std::cout << *it << " ";
    // *it = 42; // If you uncomment this, the code correctly WILL NOT COMPILE!
  }
  std::cout << std::endl;

  std::cout << "\n"
            << CYAN << "--- TEST 2: Reverse Iterators ---" << RESET
            << std::endl;
  std::cout << "Reverse iterating (Expected: 30 20 10):" << std::endl;
  for (MutantStack<int>::reverse_iterator rit = mstack1.rbegin();
       rit != mstack1.rend(); ++rit) {
    std::cout << *rit << " ";
  }
  std::cout << std::endl;

  std::cout << "\n"
            << CYAN << "--- TEST 3: Altering the Underlying Container ---"
            << RESET << std::endl;
  // std::stack normally wraps std::deque. Here we force it to wrap std::vector!
  MutantStack<int, std::vector<int> > vecStack;
  vecStack.push(42);
  vecStack.push(84);

  std::cout << "Vector Stack elements: ";
  for (MutantStack<int, std::vector<int> >::iterator it = vecStack.begin();
       it != vecStack.end(); ++it) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::cout << "\n"
            << CYAN << "--- TEST 4: Massive Stress Test (100,000 elements) ---"
            << RESET << std::endl;
  MutantStack<int> bigStack;
  std::cout << YELLOW << "[ Action ] Pushing 100,000 random integers..."
            << RESET << std::endl;

  unsigned int seed = time(NULL);
  for (int i = 0; i < 100000; i++) {
    bigStack.push(rand_r(&seed) % 100);  // Random numbers from 0 to 99
  }

  std::cout << GREEN << "✅ Size after pushing: " << bigStack.size() << RESET
            << std::endl;

  std::cout << YELLOW << "[ Action ] Popping 50,000 integers off the top..."
            << RESET << std::endl;
  for (int i = 0; i < 50000; i++) {
    bigStack.pop();
  }

  std::cout << GREEN << "✅ Size after popping: " << bigStack.size() << RESET
            << std::endl;

  std::cout
      << YELLOW
      << "[ Action ] Iterating over the remaining 50,000 to calculate sum..."
      << RESET << std::endl;
  long long sum = 0;
  for (MutantStack<int>::iterator it = bigStack.begin(); it != bigStack.end();
       ++it) {
    sum += *it;
  }

  std::cout << "Sum of remaining 50,000 elements: " << sum << std::endl;
  std::cout << GREEN
            << "✅ Iteration over 50k elements successful and instantaneous!"
            << RESET << std::endl;

  std::cout << MAGENTA << "\n========================================" << RESET
            << std::endl;
  return 0;
}
