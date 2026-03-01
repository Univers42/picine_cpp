/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fuzzy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:43:11 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 12:45:10 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#include "../Span.hpp"

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"
#define MAGENTA "\033[35m"

int main() {
  std::cout << MAGENTA << "========================================" << RESET
            << std::endl;
  std::cout << MAGENTA << "       SPAN HARDCORE FUZZ TESTER        " << RESET
            << std::endl;
  std::cout << MAGENTA << "========================================" << RESET
            << std::endl;

  std::cout << "\n"
            << CYAN << "--- TEST 1: Absolute Limits (INT_MIN to INT_MAX) ---"
            << RESET << std::endl;
  try {
    Span extremeSpan(2);
    extremeSpan.addNumber(INT_MAX);
    extremeSpan.addNumber(INT_MIN);

    std::cout << "Added INT_MAX (" << INT_MAX << ") and INT_MIN (" << INT_MIN
              << ")\n";
    // The maximum possible span is exactly UINT_MAX (4294967295)
    std::cout << "Shortest Span: " << extremeSpan.shortestSpan() << std::endl;
    std::cout << "Longest Span:  " << extremeSpan.longestSpan() << std::endl;
    if (extremeSpan.longestSpan() == UINT_MAX) {
      std::cout << GREEN << "✅ Correctly avoided Signed Integer Overflow!"
                << RESET << std::endl;
    }
  } catch (std::exception& e) {
    std::cout << RED << "Exception: " << e.what() << RESET << std::endl;
  }

  std::cout << "\n"
            << CYAN << "--- TEST 2: Homogeneous Elements (All Zeros) ---"
            << RESET << std::endl;
  try {
    Span zeroSpan(10000);              // 10,000 numbers requirement [cite: 422]
    std::vector<int> zeros(10000, 0);  // Vector filled with 10,000 zeros

    zeroSpan.addNumber(zeros.begin(), zeros.end());
    std::cout << "Added 10,000 zeros.\n";
    std::cout << "Shortest Span: " << zeroSpan.shortestSpan()
              << " (Expected: 0)\n";
    std::cout << "Longest Span:  " << zeroSpan.longestSpan()
              << " (Expected: 0)\n";
  } catch (std::exception& e) {
    std::cout << RED << "Exception: " << e.what() << RESET << std::endl;
  }

  std::cout << "\n"
            << CYAN << "--- TEST 3: Range Insertion Strict Boundaries ---"
            << RESET << std::endl;
  try {
    Span boundarySpan(10);
    std::vector<int> chunk1(5, 42);  // 5 elements
    std::vector<int> chunk2(6, 84);  // 6 elements

    std::cout << YELLOW << "Adding chunk of 5 to a Span of 10..." << RESET
              << std::endl;
    boundarySpan.addNumber(chunk1.begin(), chunk1.end());
    std::cout << GREEN << "✅ Success! Size is now 5/10." << RESET << std::endl;

    std::cout << YELLOW << "Adding chunk of 6 to the remaining 5 slots..."
              << RESET << std::endl;
    boundarySpan.addNumber(chunk2.begin(), chunk2.end());
    std::cout << RED << "❌ Should not reach here!" << RESET << std::endl;
  } catch (std::exception& e) {
    std::cout << GREEN << "✅ Successfully caught overflow: " << e.what()
              << RESET << std::endl;
  }

  std::cout << "\n"
            << CYAN
            << "--- TEST 4: True Random Negative/Positive Mix (100k) ---"
            << RESET << std::endl;
  try {
    Span fuzzedSpan(100000);
    std::vector<int> fuzzedData;
    fuzzedData.reserve(100000);

    unsigned int seed = time(NULL);
    for (int i = 0; i < 100000; i++) {
      // Generate numbers across the entire positive and negative integer
      // spectrum
      int random_val = rand_r(&seed);
      if (rand_r(&seed) % 2 == 0) random_val *= -1;
      fuzzedData.push_back(random_val);
    }

    fuzzedSpan.addNumber(fuzzedData.begin(), fuzzedData.end());
    std::cout << "100,000 heavily randomized elements inserted.\n";
    std::cout << "Shortest Span: " << fuzzedSpan.shortestSpan() << "\n";
    std::cout << "Longest Span:  " << fuzzedSpan.longestSpan() << "\n";
  } catch (std::exception& e) {
    std::cout << RED << "Exception: " << e.what() << RESET << std::endl;
  }

  std::cout << MAGENTA << "\n========================================" << RESET
            << std::endl;
  return 0;
}
