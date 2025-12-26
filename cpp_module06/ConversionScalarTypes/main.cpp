/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:45:03 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 21:45:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"

int main(int argc, char **argv) {
    if (argc == 1) {
        // Self-test mode: run a few sample literals and show outputs like your examples
        const std::string tests[] = { "0", "nan", "42.0f" };
        for (size_t i = 0; i < sizeof(tests)/sizeof(tests[0]); ++i) {
            std::cout << "./convert " << tests[i] << std::endl;
            ScalarConverter::convert(tests[i]);
            if (i + 1 < sizeof(tests)/sizeof(tests[0]))
                std::cout << std::endl;
        }
        return 0;
    }
    
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <literal>" << std::endl;
        return 1;
    }
    
    ScalarConverter::convert(argv[1]);
    
    return 0;
}