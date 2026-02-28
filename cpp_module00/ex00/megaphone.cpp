/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   megaphone.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 16:05:43 by marvin            #+#    #+#             */
/*   Updated: 2026/02/28 15:09:34 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <sstream>
#include <vector>
#include <ctime>

# define LOUD_NOISE "* LOUD AND UNBEARABLE FEEDBACK NOISE *\n"

// Color macros for terminal output
#define YELLOW "\033[33m"
#define CYAN   "\033[36m"
#define RESET  "\033[0m"

enum TfFlags {
    TF_NONE        = 0,
    TF_TRIM_LEAD   = 1u << 0,
    TF_TRIM_TRAIL  = 1u << 1,
    TF_TRIM        = TF_TRIM_LEAD | TF_TRIM_TRAIL,
    TF_COLLAPSE_WS = 1u << 2,
    TF_SKIP_CR     = 1u << 3
};

typedef int (*char_fn)(int);

const size_t CHUNK_SIZE = 8192; // 8KB stack buffer

// Helper to write the buffer in bulk and reset the index
inline void flush_buffer(char* buf, size_t& buf_idx) {
    if (buf_idx > 0) {
        std::cout.write(buf, buf_idx);
        buf_idx = 0;
    }
}

inline void buffered_transform_chunk(const char* s, char_fn fn, unsigned int flags,
                                    bool& has_started, bool& pending_space,
                                    char* buf, size_t& buf_idx) {
    unsigned char ch;
    if (!s) return;

    while (*s) {
        ch = static_cast<unsigned char>(*s++);
        if ((flags & TF_SKIP_CR) && ch == '\r') continue;

        if (std::isspace(ch)) {
            if (flags & TF_COLLAPSE_WS) {
                if (has_started || !(flags & TF_TRIM_LEAD)) pending_space = true;
            } else {
                if (has_started || !(flags & TF_TRIM_LEAD)) {
                    buf[buf_idx++] = ' ';
                    if (buf_idx >= CHUNK_SIZE) flush_buffer(buf, buf_idx);
                    has_started = true;
                }
            }
        } else {
            if (pending_space) {
                buf[buf_idx++] = ' ';
                if (buf_idx >= CHUNK_SIZE) flush_buffer(buf, buf_idx);
                pending_space = false;
            }
            buf[buf_idx++] = (fn ? static_cast<char>(fn(ch)) : static_cast<char>(ch));
            if (buf_idx >= CHUNK_SIZE) flush_buffer(buf, buf_idx);
            has_started = true;
        }
    }
}

int optimized_solution(int ac, char** av) {
    unsigned int user_flags = TF_TRIM | TF_COLLAPSE_WS;
    bool has_started = false;
    bool pending_space = false;
    char buf[CHUNK_SIZE];
    size_t buf_idx = 0;

    if (ac < 2)
        return (std::cout << YELLOW LOUD_NOISE RESET, 0);
    std::cout << CYAN;
    for (int i = 1; i < ac; ++i)
        buffered_transform_chunk(av[i], std::toupper, user_flags, has_started, pending_space, buf, buf_idx);
    if (!has_started) {
        std::cout << YELLOW LOUD_NOISE RESET;
    } else {
        if (!(user_flags & TF_TRIM_TRAIL) && pending_space) {
            buf[buf_idx++] = ' ';
            if (buf_idx >= CHUNK_SIZE) flush_buffer(buf, buf_idx);
        }
        flush_buffer(buf, buf_idx);
        std::cout << RESET << "\n";
    }
    return (0);
}
// ─────────────────────────────────────────────────────────────────────────────
//  APPROACH 2: The Simple std::string + std::transform
// ─────────────────────────────────────────────────────────────────────────────
int other_solution(int ac, char **av) {
    if (ac == 1)
        return (std::cout << YELLOW LOUD_NOISE RESET , 0);
    std::cout << CYAN;
    for (int j = 1; j < ac; ++j) {
        std::string str = av[j];
        std::transform(str.begin(), str.end(), str.begin(), ::toupper);
        std::cout << str;
    }
    std::cout << RESET << "\n";
    return 0;
}

__attribute__((weak))
int main(int ac, char **av)
{
  return (
    std::cout << "yo: ", optimized_solution(ac, av),
    std::cout << "daniel: ", other_solution(ac, av), 0);
}

