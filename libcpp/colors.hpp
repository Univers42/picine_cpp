/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/23 21:17:44 by marvin            #+#    #+#             */
/*   Updated: 2025/12/23 21:17:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_HPP
# define COLORS_HPP

#pragma once

#include <string>

namespace tester
{
    namespace colors
    {

        // Reset / styles
        inline constexpr const char *RESET = "\033[0m";
        inline constexpr const char *BOLD = "\033[1m";
        inline constexpr const char *DIM = "\033[2m";
        inline constexpr const char *ITALIC = "\033[3m";
        inline constexpr const char *UNDERLINE = "\033[4m";
        inline constexpr const char *BLINK = "\033[5m";
        inline constexpr const char *REVERSE = "\033[7m";
        inline constexpr const char *HIDDEN = "\033[8m";
        inline constexpr const char *STRIKETHROUGH = "\033[9m";

        // Standard foreground (30-37)
        inline constexpr const char *BLACK = "\033[30m";
        inline constexpr const char *RED = "\033[31m";
        inline constexpr const char *GREEN = "\033[32m";
        inline constexpr const char *YELLOW = "\033[33m";
        inline constexpr const char *BLUE = "\033[34m";
        inline constexpr const char *MAGENTA = "\033[35m";
        inline constexpr const char *CYAN = "\033[36m";
        inline constexpr const char *WHITE = "\033[37m";

        // Bright foreground (90-97)
        inline constexpr const char *BRIGHT_BLACK = "\033[90m";
        inline constexpr const char *BRIGHT_RED = "\033[91m";
        inline constexpr const char *BRIGHT_GREEN = "\033[92m";
        inline constexpr const char *BRIGHT_YELLOW = "\033[93m";
        inline constexpr const char *BRIGHT_BLUE = "\033[94m";
        inline constexpr const char *BRIGHT_MAGENTA = "\033[95m";
        inline constexpr const char *BRIGHT_CYAN = "\033[96m";
        inline constexpr const char *BRIGHT_WHITE = "\033[97m";

        // Standard background (40-47)
        inline constexpr const char *BG_BLACK = "\033[40m";
        inline constexpr const char *BG_RED = "\033[41m";
        inline constexpr const char *BG_GREEN = "\033[42m";
        inline constexpr const char *BG_YELLOW = "\033[43m";
        inline constexpr const char *BG_BLUE = "\033[44m";
        inline constexpr const char *BG_MAGENTA = "\033[45m";
        inline constexpr const char *BG_CYAN = "\033[46m";
        inline constexpr const char *BG_WHITE = "\033[47m";

        // Bright background (100-107)
        inline constexpr const char *BG_BRIGHT_BLACK = "\033[100m";
        inline constexpr const char *BG_BRIGHT_RED = "\033[101m";
        inline constexpr const char *BG_BRIGHT_GREEN = "\033[102m";
        inline constexpr const char *BG_BRIGHT_YELLOW = "\033[103m";
        inline constexpr const char *BG_BRIGHT_BLUE = "\033[104m";
        inline constexpr const char *BG_BRIGHT_MAGENTA = "\033[105m";
        inline constexpr const char *BG_BRIGHT_CYAN = "\033[106m";
        inline constexpr const char *BG_BRIGHT_WHITE = "\033[107m";

        // Helpers for 256-color and truecolor (RGB)
        // Usage: std::cout << colors::fg256(129) << "text" << colors::RESET << '\n';
        inline std::string fg256(int n)
        {
            return std::string("\033[38;5;") + std::to_string(n) + "m";
        }
        inline std::string bg256(int n)
        {
            return std::string("\033[48;5;") + std::to_string(n) + "m";
        }
        // Truecolor (24-bit): fg and bg
        inline std::string fg_rgb(int r, int g, int b)
        {
            return std::string("\033[38;2;") + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
        }
        inline std::string bg_rgb(int r, int g, int b)
        {
            return std::string("\033[48;2;") + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
        }

    } // namespace colors
} // namespace tester

#endif