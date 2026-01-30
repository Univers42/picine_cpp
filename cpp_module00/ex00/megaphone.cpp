/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:05:43 by marvin            #+#    #+#             */
/*   Updated: 2026/01/29 22:33:26 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <cstring>
#include <iostream>
#include <string>

#define LOUD_MSG "* LOUD AND UNBEARABLE FEEDBACK NOISE *"

/**
 * the function uses the boolean variable first to track
 * if any non-empty argument has been processed.
 * If first is still true after the loop, it means all arguments were empty,
 * so it returns the loud message. If at least one non-empty argument was found,
 * it builds and returns the output string.
 */
static bool extract_trimmed_upper(const char* s, std::string& token) {
  size_t j = 0;
  bool started = false;
  size_t last_non_space = 0;
  unsigned char c;

  token.clear();
  for (; s[j] != '\0'; ++j) {
    c = static_cast<unsigned char>(s[j]);
    if (!started) {
      if (std::isspace(c)) continue;
      started = true;
    }
    token.push_back(static_cast<char>(std::toupper(c)));
    if (!std::isspace(c)) last_non_space = token.size();
  }
  if (!started) return (token.clear(), false);
  token.resize(last_non_space);
  return (true);
}

static void append_with_sep(std::string& out, const std::string& token) {
  if (!out.empty()) out.push_back(' ');
  out += token;
}

std::string megaphone_from_argv(int n, char* args[]) {
  char* s;

  if (n <= 0) return (LOUD_MSG);
  if (args == NULL) return (LOUD_MSG);
  std::string out;
  std::string token;
  for (int ai = 0; ai < n; ++ai) {
    s = args[ai];
    if (s == NULL) continue;
    if (!extract_trimmed_upper(s, token)) continue;
    append_with_sep(out, token);
  }
  if (out.empty()) return (LOUD_MSG);
  return (out);
}

void print_line(const char* s) { std::cout << s << '\n'; }
static void print_line(const std::string& s) { std::cout << s << '\n'; }

std::string megaphone_from_argv(int n, char* args[]);

int main(int argc, char** argv) {
  std::string out = megaphone_from_argv(argc - 1, argv + 1);
  print_line(out);
  return 0;
}
