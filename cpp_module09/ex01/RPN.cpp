/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 16:17:46 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 16:35:55 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>

RPN::RPN() {}

RPN::RPN(const RPN& src) : _stack(src._stack) {}

RPN& RPN::operator=(const RPN& rhs) {
  if (this != &rhs) {
    this->_stack = rhs._stack;
  }
  return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(const std::string& token) const {
  return (token == "+" || token == "-" || token == "*" || token == "/");
}

void RPN::applyOperation(const std::string& op) {
  if (_stack.size() < 2) {
    throw std::runtime_error("Error: Not enough operands.");
  }

  // The first popped element is the RIGHT operand!
  int right = _stack.top();
  _stack.pop();
  int left = _stack.top();
  _stack.pop();

  if (op == "+") {
    _stack.push(left + right);
  } else if (op == "-") {
    _stack.push(left - right);
  } else if (op == "*") {
    _stack.push(left * right);
  } else if (op == "/") {
    if (right == 0) {
      throw std::runtime_error("Error: Division by zero.");
    }
    _stack.push(left / right);
  }
}

void RPN::evaluate(const std::string& expr) {
  std::istringstream iss(expr);
  std::string token;

  while (iss >> token) {
    if (token.length() == 1 && std::isdigit(token[0])) {
      // The subject guarantees numbers will always be less than 10 (single
      // digits)
      _stack.push(token[0] - '0');
    } else if (token.length() == 1 && isOperator(token)) {
      applyOperation(token);
    } else {
      throw std::runtime_error("Error");
    }
  }

  // Once evaluation is complete, there must be exactly 1 item left in the
  // stack.
  if (_stack.size() != 1) {
    throw std::runtime_error("Error: Invalid expression format.");
  }

  std::cout << _stack.top() << std::endl;
}
