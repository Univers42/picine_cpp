/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:00:20 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:11:53 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Zombie.hpp"

int main(void) {
  std::cout << C_BOLD << C_MAGENTA
            << "\n=== 1. TESTING STACK ALLOCATION (randomChump) ===" << C_RESET
            << std::endl;
  std::cout << C_YELLOW << "-> Calling randomChump(\"StackRunner\")" << C_RESET
            << std::endl;
  randomChump("StackRunner");
  std::cout << C_DIM
            << "-> Notice how StackRunner was automatically destroyed right "
               "after announcing itself!"
            << C_RESET << std::endl;

  std::cout << C_BOLD << C_MAGENTA
            << "\n=== 2. TESTING HEAP ALLOCATION (newZombie) ===" << C_RESET
            << std::endl;
  std::cout << C_YELLOW << "-> Calling newZombie(\"HeapWalker\")" << C_RESET
            << std::endl;
  Zombie* heapWalker = newZombie("HeapWalker");

  std::cout << C_YELLOW
            << "-> Making HeapWalker announce itself from main scope:"
            << C_RESET << std::endl;
  heapWalker->announce();

  std::cout << C_YELLOW << "-> Deleting HeapWalker manually:" << C_RESET
            << std::endl;
  delete heapWalker;
  std::cout << C_DIM << "-> HeapWalker is now completely destroyed." << C_RESET
            << std::endl;

  std::cout << C_BOLD << C_MAGENTA
            << "\n=== 3. TESTING EDGE CASES ===" << C_RESET << std::endl;

  std::cout << C_YELLOW << "\n[Edge Case A: Empty Name]" << C_RESET
            << std::endl;
  randomChump("");

  std::cout << C_YELLOW << "\n[Edge Case B: Special Characters]" << C_RESET
            << std::endl;
  Zombie* special = newZombie("Z0mb!e_#42");
  special->announce();
  delete special;

  std::cout << C_YELLOW << "\n[Edge Case C: Very Long Name]" << C_RESET
            << std::endl;
  randomChump("HubertBlaineWolfeschlegelsteinhausenbergerdorffSr");

  std::cout << C_BOLD << C_MAGENTA << "\n=== END OF TESTS ===" << C_RESET
            << "\n"
            << std::endl;

  return 0;
}
