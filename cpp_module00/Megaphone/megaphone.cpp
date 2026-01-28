/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:05:43 by marvin            #+#    #+#             */
/*   Updated: 2026/01/28 18:16:16 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstring>
#include <iostream>
#include <string>

static void print_line(const char* s) { std::cout << s << '\n'; }
static void print_line(const std::string& s) { std::cout << s << '\n'; }

int main(int argc, char** argv) {
  const char* fb = "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
  char* s;
  bool first = true;
  unsigned char ch;

  if (argc <= 1) return (print_line(fb), 0);
  std::string out;
  for (int i = 1; i < argc; ++i) {
    if (!first) out.push_back(' ');
    first = false;
    s = argv[i];
    for (std::size_t j = 0; s[j] != '\0'; ++j) {
      ch = static_cast<unsigned char>(s[j]);
      out.push_back(static_cast<char>(std::toupper(ch)));
    }
  }
  return (print_line(out), 0);
}
