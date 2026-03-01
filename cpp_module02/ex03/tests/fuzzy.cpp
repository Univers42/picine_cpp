/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fuzzy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 21:54:29 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:06:22 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#include "../Point.hpp"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

int testsTotal = 0;
int testsPassed = 0;

void print_result(std::string test_name, bool expected, bool actual) {
  testsTotal++;
  std::cout << "Test " << test_name << ": ";
  if (expected == actual) {
    testsPassed++;
    std::cout << GREEN << "PASS" << RESET << std::endl;
  } else {
    std::cout << RED << "FAIL" << RESET << std::endl;
  }
}

// ─── FUZZER HELPER FUNCTIONS ────────────────────────────────────────────────

// Generate random float between min and max
float getRandomFloat(float min, float max) {
  static unsigned int seed = 42;
  float scale =
      static_cast<float>(rand_r(&seed)) / static_cast<float>(RAND_MAX);
  return min + scale * (max - min);
}

// A standard 32-bit floating point cross product to act as the "Source of
// Truth"
float floatCrossProduct(float px, float py, float a_x, float a_y, float b_x,
                        float b_y) {
  return (px - b_x) * (a_y - b_y) - (a_x - b_x) * (py - b_y);
}

// A standard 32-bit float BSP logic
bool standard_bsp(float ax, float ay, float bx, float by, float cx, float cy,
                  float px, float py) {
  float d1 = floatCrossProduct(px, py, ax, ay, bx, by);
  float d2 = floatCrossProduct(px, py, bx, by, cx, cy);
  float d3 = floatCrossProduct(px, py, cx, cy, ax, ay);

  // If the point is extremely close to the edge, we skip comparing it
  // because 8-bit Fixed math and 32-bit Float math handle tiny edge boundaries
  // differently.
  if (std::abs(d1) < 0.1f || std::abs(d2) < 0.1f || std::abs(d3) < 0.1f)
    return false;  // Treat as edge (outside)

  bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
  bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

  return !(has_neg && has_pos);
}

// ─── FUZZING ENGINE ─────────────────────────────────────────────────────────

void run_fuzzer(int iterations) {
  std::cout << BOLD << "\n--- Running " << iterations << " BSP Fuzz Tests ---"
            << RESET << std::endl;

  // Create a large, random triangle to test against
  Point a(getRandomFloat(-50.0f, 0.0f), getRandomFloat(-50.0f, 0.0f));
  Point b(getRandomFloat(0.0f, 50.0f), getRandomFloat(-50.0f, 0.0f));
  Point c(getRandomFloat(-25.0f, 25.0f), getRandomFloat(0.0f, 50.0f));

  // Extract the quantized (Fixed) values so the float algorithm sees exactly
  // what the Fixed algorithm sees
  float ax = a.getX().toFloat();
  float ay = a.getY().toFloat();
  float bx = b.getX().toFloat();
  float by = b.getY().toFloat();
  float cx = c.getX().toFloat();
  float cy = c.getY().toFloat();

  int fuzz_passed = 0;
  int fuzz_total = 0;

  for (int i = 0; i < iterations; i++) {
    // Generate a random point anywhere in the bounding box area
    float px = getRandomFloat(-60.0f, 60.0f);
    float py = getRandomFloat(-60.0f, 60.0f);
    Point p(px, py);

    // Get the quantized float equivalents
    float qpx = p.getX().toFloat();
    float qpy = p.getY().toFloat();

    // Check if it's too close to the edge to be a fair test between 8-bit and
    // 32-bit
    float d1 = floatCrossProduct(qpx, qpy, ax, ay, bx, by);
    float d2 = floatCrossProduct(qpx, qpy, bx, by, cx, cy);
    float d3 = floatCrossProduct(qpx, qpy, cx, cy, ax, ay);
    if (std::abs(d1) < 0.5f || std::abs(d2) < 0.5f || std::abs(d3) < 0.5f) {
      continue;  // Skip boundary points to avoid precision-based false
                 // negatives
    }

    fuzz_total++;
    testsTotal++;

    bool expected = standard_bsp(ax, ay, bx, by, cx, cy, qpx, qpy);
    bool actual = bsp(a, b, c, p);

    if (expected == actual) {
      fuzz_passed++;
      testsPassed++;
    } else {
      std::cout << RED << "[FUZZ FAIL] " << RESET << "Point(" << px << ", "
                << py << ") "
                << "| Expected: " << (expected ? "Inside" : "Outside")
                << " | Actual: " << (actual ? "Inside" : "Outside")
                << std::endl;
    }
  }

  if (fuzz_passed == fuzz_total)
    std::cout << GREEN << "[SUCCESS] " << fuzz_passed << "/" << fuzz_total
              << " Fuzz Tests Passed!" << RESET << std::endl;
  else
    std::cout << RED << "[FAILED] " << (fuzz_total - fuzz_passed)
              << " Fuzz Tests Failed." << RESET << std::endl;
}

// ─── MAIN ───────────────────────────────────────────────────────────────────

int main(void) {
  std::cout << BOLD << "--- BSP Edge Case Tests ---" << RESET << std::endl;

  Point a(0.0f, 0.0f);
  Point b(10.0f, 0.0f);
  Point c(0.0f, 10.0f);

  Point inside(2.0f, 2.0f);
  print_result("Strictly Inside", true, bsp(a, b, c, inside));

  Point outside(10.0f, 10.0f);
  print_result("Strictly Outside", false, bsp(a, b, c, outside));

  Point on_edge(5.0f, 5.0f);
  print_result("On Edge", false, bsp(a, b, c, on_edge));

  Point on_vertex(0.0f, 10.0f);
  print_result("On Vertex", false, bsp(a, b, c, on_vertex));

  Point a2(-5.0f, -5.0f);
  Point b2(5.0f, -5.0f);
  Point c2(0.0f, 5.0f);
  Point inside_neg(0.0f, 0.0f);
  print_result("Inside Negative Triangle", true, bsp(a2, b2, c2, inside_neg));

  // Run the massive fuzzing test!
  run_fuzzer(10000);

  std::cout << BOLD << "\n--- FINAL RESULTS ---" << RESET << std::endl;
  if (testsPassed == testsTotal) {
    std::cout << GREEN << "All " << testsPassed
              << " tests perfectly simulate FPU BSP algorithms!" << RESET
              << std::endl;
  } else {
    std::cout << RED << "Only " << testsPassed << "/" << testsTotal
              << " tests passed." << RESET << std::endl;
  }

  return 0;
}
