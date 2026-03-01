/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 12:55:20 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 12:59:03 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>
#include <stack>

#include "MutantStack.hpp"

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define CYAN "\033[36m"

int main() {
  std::cout << CYAN << "========================================" << RESET
            << std::endl;
  std::cout << "--- 1. Mandatory Subject Test (MutantStack) ---" << std::endl;
  std::cout << CYAN << "========================================" << RESET
            << std::endl;

  MutantStack<int> mstack;

  mstack.push(5);
  mstack.push(17);
  std::cout << "Top element: " << mstack.top() << std::endl;

  mstack.pop();
  std::cout << "Size after pop: " << mstack.size() << std::endl;

  mstack.push(3);
  mstack.push(5);
  mstack.push(737);
  //[...]
  mstack.push(0);

  MutantStack<int>::iterator it = mstack.begin();
  MutantStack<int>::iterator ite = mstack.end();

  ++it;
  --it;

  std::cout << GREEN << "\nIterating through MutantStack:" << RESET
            << std::endl;
  while (it != ite) {
    std::cout << *it << std::endl;
    ++it;
  }
  std::stack<int> s(mstack);  // Proves it is still a valid std::stack!

  std::cout << "\n"
            << CYAN << "========================================" << RESET
            << std::endl;
  std::cout << "--- 2. Comparison Test (std::list) ---" << std::endl;
  std::cout << CYAN << "========================================" << RESET
            << std::endl;

  std::list<int> mlist;

  mlist.push_back(5);
  mlist.push_back(17);
  std::cout << "Top element: " << mlist.back() << std::endl;

  mlist.pop_back();
  std::cout << "Size after pop: " << mlist.size() << std::endl;

  mlist.push_back(3);
  mlist.push_back(5);
  mlist.push_back(737);
  //[...]
  mlist.push_back(0);

  std::list<int>::iterator lit = mlist.begin();
  std::list<int>::iterator lite = mlist.end();

  ++lit;
  --lit;

  std::cout << GREEN << "\nIterating through std::list:" << RESET << std::endl;
  while (lit != lite) {
    std::cout << *lit << std::endl;
    ++lit;
  }

  return 0;
}
