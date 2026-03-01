/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rpn_generator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 16:52:09 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 16:53:24 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* rpn_generator.cpp                                  :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2026/03/01 16:45:30 by dlesieur          #+#    #+#             */
/* Updated: 2026/03/01 16:45:31 by dlesieur         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include <stdint.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
std::string to_string(T val) {
  std::stringstream ss;
  ss << val;
  return ss.str();
}

int main(int argc, char** argv) {
  int num_tests = 1000;
  if (argc > 1) num_tests = std::atoi(argv[1]);
  std::srand(std::time(NULL));
  const char ops[] = {'+', '-', '*', '/'};
  for (int i = 0; i < num_tests; ++i) {
    std::vector<int64_t> stack;
    std::string expr = "";
    int target_len = (std::rand() % 20) + 3;
    int numbers_added = 0;
    int ops_added = 0;

    while (numbers_added + ops_added < target_len || stack.size() > 1) {
      bool add_num = (stack.size() < 2) || (std::rand() % 2 == 0);
      if (numbers_added + ops_added >= target_len && stack.size() > 1) {
        add_num = false;
      }

      if (add_num) {
        int64_t n = std::rand() % 10;
        stack.push_back(n);
        if (!expr.empty()) expr += " ";
        expr += to_string(n);
        numbers_added++;
      } else {
        char op = ops[std::rand() % 4];
        int64_t right = stack.back();
        stack.pop_back();
        int64_t left = stack.back();
        stack.pop_back();
        if (op == '/' && right == 0) {
          op = '+';
        }

        if (op == '+')
          stack.push_back(left + right);
        else if (op == '-')
          stack.push_back(left - right);
        else if (op == '*')
          stack.push_back(left * right);
        else if (op == '/')
          stack.push_back(left / right);

        if (!expr.empty()) expr += " ";
        expr += op;
        ops_added++;
      }
    }
    std::cout << expr << "|" << stack.back() << "\n";
  }
  return 0;
}
