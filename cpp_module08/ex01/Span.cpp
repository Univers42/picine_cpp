/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:30:14 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 17:18:20 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Span.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include <vector>  // cpplint requires explicitly including what you use

Span::Span() : _maxSize(0) {}

Span::Span(unsigned int N) : _maxSize(N) { _vec.reserve(N); }

Span::Span(const Span& src) : _maxSize(src._maxSize), _vec(src._vec) {}

Span& Span::operator=(const Span& rhs) {
  if (this != &rhs) {
    this->_maxSize = rhs._maxSize;
    this->_vec = rhs._vec;
  }
  return *this;
}

Span::~Span() {}

void Span::addNumber(int n) {
  if (_vec.size() >= _maxSize) throw SpanFullException();  // Added ()
  _vec.push_back(n);
}

unsigned int Span::longestSpan() const {
  if (_vec.size() < 2) throw NotEnoughElementsException();

  int minVal = *std::min_element(_vec.begin(), _vec.end());
  int maxVal = *std::max_element(_vec.begin(), _vec.end());

  // to prevent signed overflow!
  return static_cast<unsigned int>(static_cast<int64_t>(maxVal) -
                                   static_cast<int64_t>(minVal));
}

unsigned int Span::shortestSpan() const {
  if (_vec.size() < 2) throw NotEnoughElementsException();

  std::vector<int> sortedVec = _vec;
  std::sort(sortedVec.begin(), sortedVec.end());

  unsigned int minSpan = std::numeric_limits<unsigned int>::max();

  for (size_t i = 1; i < sortedVec.size(); ++i) {
    // to prevent signed overflow!
    unsigned int diff =
        static_cast<unsigned int>(static_cast<int64_t>(sortedVec[i]) -
                                  static_cast<int64_t>(sortedVec[i - 1]));
    if (diff < minSpan) minSpan = diff;
  }

  return minSpan;
}

const char* Span::SpanFullException::what() const throw() {
  return "Error: Span is already full, cannot add more numbers.";
}

const char* Span::NotEnoughElementsException::what() const throw() {
  return "Error: Not enough elements to calculate a span (requires at least "
         "2).";
}
