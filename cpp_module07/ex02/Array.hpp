/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Array.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:04:33 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 12:08:38 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE07_EX02_ARRAY_HPP_
#define CPP_MODULE07_EX02_ARRAY_HPP_

#include <exception>
#include <iostream>

template <typename T>
class Array {
 private:
  T* _array;
  unsigned int _size;

 public:
  Array() : _array(NULL), _size(0) {}

  explicit Array(unsigned int n) : _array(new T[n]()), _size(n) {}

  Array(const Array& src) : _array(NULL), _size(src._size) {
    if (_size > 0) {
      _array = new T[_size]();
      for (unsigned int i = 0; i < _size; ++i) _array[i] = src._array[i];
    }
  }

  Array& operator=(const Array& rhs) {
    if (this != &rhs) {
      delete[] _array;
      _size = rhs._size;
      if (_size > 0) {
        _array = new T[_size]();
        for (unsigned int i = 0; i < _size; ++i) _array[i] = rhs._array[i];
      } else {
        _array = NULL;
      }
    }
    return (*this);
  }

  ~Array() { delete[] _array; }

  T& operator[](unsigned int index) {
    if (index >= _size) throw OutOfBoundsException();
    return (_array[index]);
  }

  const T& operator[](unsigned int index) const {
    if (index >= _size) throw OutOfBoundsException();
    return (_array[index]);
  }

  unsigned int size() const { return (_size); }

  class OutOfBoundsException : public std::exception {
   public:
    virtual const char* what() const throw() {
      return "Error: Array index is out of bounds!";
    }
  };
};

#endif  // CPP_MODULE07_EX02_ARRAY_HPP_
