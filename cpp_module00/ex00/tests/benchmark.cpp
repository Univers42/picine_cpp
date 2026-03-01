/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 14:20:04 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:16:32 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <cctype>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../megaphone_solutions.hpp"

typedef int (*SolutionFn)(int, char**);

double benchmark_solution(const std::string& name, SolutionFn func, int ac,
                          char** av) {
  std::streambuf* old_cout = std::cout.rdbuf();
  std::ostringstream oss;
  std::cout.rdbuf(oss.rdbuf());

  std::clock_t start = std::clock();
  func(ac, av);
  std::clock_t end = std::clock();

  std::cout.rdbuf(old_cout);

  double elapsed_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;
  std::cout << "[Result] " << name << " -> Time: " << elapsed_ms
            << " ms | Output Length: " << oss.str().length() << " chars\n";

  return elapsed_ms;
}

char* generate_random_string(int length, int space_percentage) {
  char* str = new char[length + 1];
  for (int i = 0; i < length; ++i) {
    int rand_val = std::rand() % 100;
    if (rand_val < space_percentage) {
      int ws_type = std::rand() % 3;
      str[i] = (ws_type == 0) ? ' ' : ((ws_type == 1) ? '\t' : '\r');
    } else {
      str[i] = 'a' + (std::rand() % 26);
    }
  }
  str[length] = '\0';
  return str;
}

void run_chaos_scenario(const std::string& name, int num_args,
                        int length_per_arg, int space_pct) {
  std::cout << "\n───────────────────────────────────────────\n";
  std::cout << " Scenario: " << name << "\n";
  std::cout << " Payload : " << num_args << " args x " << length_per_arg
            << " chars (~" << (num_args * length_per_arg) / 1000000.0
            << " MB)\n";
  std::cout << " Chaos   : " << space_pct << "% whitespace\n";
  std::cout << "───────────────────────────────────────────\n";

  std::vector<char*> argv;
  char prog_name[] = "./megaphone";
  argv.push_back(prog_name);

  for (int i = 0; i < num_args; ++i) {
    argv.push_back(generate_random_string(length_per_arg, space_pct));
  }

  int ac = static_cast<int>(argv.size());

  benchmark_solution("Other Solution (std::string)", other_solution, ac,
                     &argv[0]);
  benchmark_solution("Optimized Streamer (In-Place)", optimized_solution, ac,
                     &argv[0]);

  for (size_t i = 1; i < argv.size(); ++i) {
    delete[] argv[i];
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::srand(42);

  std::cout << "═══════════════════════════════════════════\n";
  std::cout << "  Megaphone 'Millions of Bytes' Benchmark\n";
  std::cout << "═══════════════════════════════════════════\n";
  run_chaos_scenario("The 5MB Monolith", 1, 5000000, 2);
  run_chaos_scenario("The Fragmentation Nightmare", 50000, 100, 25);
  run_chaos_scenario("The Space Desert", 2000, 1000, 90);
  return 0;
}
