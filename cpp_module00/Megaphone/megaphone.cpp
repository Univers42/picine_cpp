/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:05:43 by marvin            #+#    #+#             */
/*   Updated: 2025/12/23 21:54:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libcpp.h"

int main(int argc, char **argv) {
  const char *fb = "* LOUD AND UNBEARABLE FEEDBACK NOISE *";
  std::string out;
  bool first = true;

  if (argc == 1)
    return (print_line(fb), 0);
  for (char **arg = argv + 1; *arg != NULL; ++arg) {
    std::string temp(*arg);
    toupperstr(&temp);
    if (!first) out.append(" ");
    out.append(temp);
    first = false;
  }
  return (print_line(out), 0);
}
