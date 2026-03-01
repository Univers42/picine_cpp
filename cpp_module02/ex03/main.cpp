/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 21:49:55 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:06:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "Point.hpp"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

void print_result(std::string test_name, bool expected, bool actual) {
  std::cout << "Test " << test_name << ": ";
  if (expected == actual)
    std::cout << GREEN << "PASS" << RESET << std::endl;
  else
    std::cout << RED << "FAIL" << RESET << std::endl;
}

__attribute__((weak)) int main(void) {
  // Triangle vertices
  Point a(0.0f, 0.0f);
  Point b(10.0f, 0.0f);
  Point c(0.0f, 10.0f);

  std::cout << "--- BSP Point in Triangle Tests ---" << std::endl;

  // Test 1: Strictly Inside
  Point inside(2.0f, 2.0f);
  print_result("Strictly Inside", true, bsp(a, b, c, inside));

  // Test 2: Strictly Outside
  Point outside(10.0f, 10.0f);
  print_result("Strictly Outside", false, bsp(a, b, c, outside));

  // Test 3: On the Edge (Hypotenuse)
  Point on_edge(5.0f, 5.0f);
  print_result("On Edge", false, bsp(a, b, c, on_edge));

  // Test 4: Exactly on a Vertex
  Point on_vertex(0.0f, 10.0f);  // Exactly point C
  print_result("On Vertex", false, bsp(a, b, c, on_vertex));

  // Test 5: Negative Coordinates Triangle
  Point a2(-5.0f, -5.0f);
  Point b2(5.0f, -5.0f);
  Point c2(0.0f, 5.0f);
  Point inside_neg(0.0f, 0.0f);
  print_result("Inside Negative Triangle", true, bsp(a2, b2, c2, inside_neg));

  return 0;
}
