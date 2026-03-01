/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 17:01:13 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:16:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>

#include <cstdlib>
#include <deque>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "PmergeMe.hpp"

#define RED "\033[91m"
#define RESET "\033[0m"

template <typename Iterator>
bool is_safely_sorted(Iterator first, Iterator last) {
  if (first == last) return true;
  Iterator next = first;
  while (++next != last) {
    if (*next < *first) return false;
    ++first;
  }
  return true;
}

// Safely converts microsecond times to doubles
double get_time_in_us() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000000.0) + tv.tv_usec;
}

// Helper to print a container up to a limit
template <typename Container>
void print_container(const std::string& prefix, const Container& c) {
  std::cout << prefix;
  size_t limit = c.size() > 5 ? 5 : c.size();
  for (size_t i = 0; i < limit; ++i) {
    std::cout << c[i] << " ";
  }
  if (c.size() > 5) std::cout << "[...]";
  std::cout << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Error: No sequence provided." << std::endl;
    return 1;
  }

  std::vector<int> vec;
  std::deque<int> deq;
  try {
    for (int i = 1; i < argc; ++i) {
      std::string arg(argv[i]);
      if (arg.empty()) throw std::runtime_error("Empty argument found.");
      std::istringstream iss(arg);
      std::string token;
      while (iss >> token) {
        for (size_t j = 0; j < token.size(); ++j) {
          if (!std::isdigit(token[j])) {
            if (j == 0 && token[j] == '+' && token.size() > 1) continue;
            throw std::runtime_error("Invalid character in sequence.");
          }
        }
        int64_t val = std::atol(token.c_str());
        if (val < 0 || val > 2147483647) {
          throw std::runtime_error("Number out of valid integer bounds.");
        }

        vec.push_back(static_cast<int>(val));
        deq.push_back(static_cast<int>(val));
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error" << std::endl;
    return 1;
  }
  if (vec.size() < 2) {
    std::cerr << "Error: Not enough elements to sort." << std::endl;
    return 1;
  }
  PmergeMe sorter;
  print_container("Before: ", vec);
  double start_vec = get_time_in_us();
  sorter.sortVector(vec);
  double end_vec = get_time_in_us();
  double time_vec = end_vec - start_vec;
  double start_deq = get_time_in_us();
  sorter.sortDeque(deq);
  double end_deq = get_time_in_us();
  double time_deq = end_deq - start_deq;
  if (!is_safely_sorted(vec.begin(), vec.end())) {
    std::cerr << RED << "CRITERR: std::vector is NOT sorted correctly!"
              << std::endl;
    return (1);
  }
  if (!is_safely_sorted(deq.begin(), deq.end())) {
    std::cerr << RED << "CRITERR: std::deque is NOT sorted correctly!"
              << std::endl;
    return (1);
  }
  if (vec.size() != deq.size()) {
    std::cerr << RED << "CRITERR: Data loss detected during sort!" << RESET
              << std::endl;
    return (1);
  }
  print_container("After:  ", vec);
  std::cout << std::fixed << std::setprecision(5);
  std::cout << "Time to process a range of " << vec.size()
            << " elements with std::vector : " << time_vec << " us"
            << std::endl;
  std::cout << "Time to process a range of " << deq.size()
            << " elements with std::deque  : " << time_deq << " us"
            << std::endl;

  return 0;
}
