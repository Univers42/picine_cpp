#include <cmath>
#include <iostream>
#include <string>

#include "Fixed.hpp"

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

// Structure to map test names to functions (Dictionary Pattern)
struct TestMapping {
  const char* name;
  void (*func)(void);
};

// --- Test Functions ---

void test_subject_values() {
  Fixed const b(10);
  Fixed const c(42.42f);

  if (std::abs(b.toFloat() - 10.0f) < 0.0001f &&
      std::abs(c.toFloat() - 42.421875f) < 0.0001f)
    std::cout << GREEN << "  [OK] Subject basic conversions" << RESET
              << std::endl;
  else
    std::cout << RED << "  [KO] Subject basic conversions" << RESET
              << std::endl;
}

void test_precision_limits() {
  Fixed min_step;
  min_step.setRawBits(1);  // Smallest possible value

  if (std::abs(min_step.toFloat() - 0.00390625f) < 0.00000001f)
    std::cout << GREEN << "  [OK] Precision delta (1/256)" << RESET
              << std::endl;
  else
    std::cout << RED << "  [KO] Precision delta" << RESET << std::endl;
}

void test_negative_values() {
  Fixed neg_int(-10);
  Fixed neg_float(-42.42f);

  if (neg_int.toInt() == -10 && neg_float.toInt() == -42)
    std::cout << GREEN << "  [OK] Negative value handling" << RESET
              << std::endl;
  else
    std::cout << RED << "  [KO] Negative value handling" << RESET << std::endl;
}

void test_large_values() {
  // Max value for 8-bit fractional part in a 32-bit signed int:
  // (2^31 - 1) / 256 = ~8,388,607
  Fixed large(8000000);
  if (large.toInt() == 8000000)
    std::cout << GREEN << "  [OK] Large integer handling" << RESET << std::endl;
  else
    std::cout << RED << "  [KO] Large integer handling" << RESET << std::endl;
}

// --- Test Runner ---

int main(void) {
  TestMapping dispatchTable[] = {{"Subject Values", test_subject_values},
                                 {"Precision Limits", test_precision_limits},
                                 {"Negative Values", test_negative_values},
                                 {"Large Values", test_large_values}};

  int numTests = sizeof(dispatchTable) / sizeof(TestMapping);

  std::cout << BOLD << "\n--- EX01 COMPREHENSIVE TEST SUITE ---" << RESET
            << std::endl;
  for (int i = 0; i < numTests; i++) {
    std::cout << "Running: " << dispatchTable[i].name << std::endl;
    dispatchTable[i].func();
  }
  std::cout << BOLD << "--------------------------------------\n"
            << RESET << std::endl;

  return 0;
}
