/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:39:37 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 12:42:18 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "Span.hpp"

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"

int main() {
  std::cout << CYAN << "========================================" << RESET
            << std::endl;
  std::cout << "--- 1. Mandatory Subject Test ---" << std::endl;
  std::cout << CYAN << "========================================" << RESET
            << std::endl;

  Span sp = Span(5);
  sp.addNumber(6);
  sp.addNumber(3);
  sp.addNumber(17);
  sp.addNumber(9);
  sp.addNumber(11);

  std::cout << "Shortest Span: " << sp.shortestSpan() << std::endl;
  std::cout << "Longest Span:  " << sp.longestSpan() << std::endl;

  std::cout << "\n"
            << CYAN << "========================================" << RESET
            << std::endl;
  std::cout << "--- 2. Exception Handling Tests ---" << std::endl;
  std::cout << CYAN << "========================================" << RESET
            << std::endl;

  try {
    std::cout << YELLOW
              << "[ Action ] Trying to add a 6th number to a Span of 5..."
              << RESET << std::endl;
    sp.addNumber(42);
  } catch (std::exception& e) {  // Fixed ampersand spacing
    std::cout << RED << e.what() << RESET << std::endl;
  }

  try {
    Span smallSpan = Span(1);
    smallSpan.addNumber(100);
    std::cout << YELLOW << "[ Action ] Trying to get span with only 1 number..."
              << RESET << std::endl;
    smallSpan.shortestSpan();
  } catch (std::exception& e) {  // Fixed ampersand spacing
    std::cout << RED << e.what() << RESET << std::endl;
  }

  std::cout << "\n"
            << CYAN << "========================================" << RESET
            << std::endl;
  std::cout << "--- 3. MASSIVE STL Range Insertion Test (100,000 elements) ---"
            << std::endl;
  std::cout << CYAN << "========================================" << RESET
            << std::endl;

  Span bigSpan = Span(100000);
  std::vector<int> randomNumbers;

  // Implemented thread-safe rand_r
  unsigned int seed = time(NULL);
  for (int i = 0; i < 100000; ++i) {
    randomNumbers.push_back(rand_r(&seed) % 1000000);
  }

  std::cout << YELLOW
            << "[ Action ] Inserting 100,000 numbers at once via iterators..."
            << RESET << std::endl;
  bigSpan.addNumber(randomNumbers.begin(), randomNumbers.end());

  std::cout << GREEN << "Insertion successful!" << RESET << std::endl;
  std::cout << "Shortest Span in 100k elements: " << bigSpan.shortestSpan()
            << std::endl;
  std::cout << "Longest Span in 100k elements:  " << bigSpan.longestSpan()
            << std::endl;

  return 0;
}
