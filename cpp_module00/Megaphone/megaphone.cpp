/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:05:43 by marvin            #+#    #+#             */
/*   Updated: 2026/01/28 18:50:54 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstring>
#include <iostream>
#include <string>

void print_line(const char* s) { std::cout << s << '\n'; }
static void print_line(const std::string& s) { std::cout << s << '\n'; }

// New testable function (C++98-safe, no vectors)
std::string megaphone_from_argv(int n, char* args[]) {
  if (n <= 0) return "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
  std::string out;
  bool first = true;
  for (int ai = 0; ai < n; ++ai) {
    if (!first) out.push_back(' ');
    first = false;
    char* s = args[ai];
    for (std::size_t j = 0; s[j] != '\0'; ++j) {
      unsigned char ch = static_cast<unsigned char>(s[j]);
      out.push_back(static_cast<char>(std::toupper(ch)));
    }
  }
  return out;
}

__attribute__((weak))
int main(int argc, char** argv) {
  std::string out = megaphone_from_argv(argc - 1, argv + 1);
  print_line(out);
  return 0;
}

