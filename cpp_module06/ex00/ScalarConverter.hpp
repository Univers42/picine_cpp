/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:43:38 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 21:43:38 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCALARCONVERTER_HPP
#define SCALARCONVERTER_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <limits>
#include <cmath>
#include <cctype>
#include <cstdlib>

class ScalarConverter {
private:
    ScalarConverter();
    ScalarConverter(const ScalarConverter& other);
    ScalarConverter& operator=(const ScalarConverter& other);
    ~ScalarConverter();

    enum LiteralType {
        CHAR_LITERAL,
        INT_LITERAL,
        FLOAT_LITERAL,
        DOUBLE_LITERAL,
        PSEUDO_LITERAL,
        INVALID
    };

    struct LiteralValue {
        LiteralType type;
        union {
            char c;
            int i;
            float f;
            double d;
        } value;
    };

    // Type detection
    static LiteralType detectType(const std::string& literal);
    static bool matchesCharPattern(const std::string& s);
    static bool matchesIntPattern(const std::string& s);
    static bool matchesFloatPattern(const std::string& s);
    static bool matchesDoublePattern(const std::string& s);
    static bool matchesPseudoPattern(const std::string& s);

    // Parsing
    static LiteralValue parse(const std::string& literal);
    
    // Char output helpers
    static void printCharFromValue(double val);
    static void printIntFromValue(double val);
    static void printFloatFromValue(double val);
    static void printDoubleFromValue(double val);
    
    // Conversion strategies
    static void outputAllTypes(double val);

public:
    static void convert(const std::string& literal);
};

#endif