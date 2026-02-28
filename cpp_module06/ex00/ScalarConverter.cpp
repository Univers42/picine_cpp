/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:43:40 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 21:43:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter&) {}
ScalarConverter& ScalarConverter::operator=(const ScalarConverter&) { return *this; }
ScalarConverter::~ScalarConverter() {}

// Pattern matching with early returns
bool ScalarConverter::matchesCharPattern(const std::string& s) {
    // Accept quoted char like 'a' OR a single printable non-digit character passed without quotes
    if (s.length() == 3 && s[0] == '\'' && s[2] == '\'')
        return true;
    if (s.length() == 1 && std::isprint(static_cast<unsigned char>(s[0])) && !std::isdigit(static_cast<unsigned char>(s[0])))
        return true;
    return false;
}

bool ScalarConverter::matchesIntPattern(const std::string& s) {
    if (s.empty()) return false;
    
    size_t start = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    if (start >= s.length()) return false;
    
    for (size_t i = start; i < s.length(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
    }
    return true;
}

bool ScalarConverter::matchesFloatPattern(const std::string& s) {
    if (s.length() < 2 || s[s.length() - 1] != 'f') return false;
    
    std::string num = s.substr(0, s.length() - 1);
    size_t start = (num[0] == '+' || num[0] == '-') ? 1 : 0;
    if (start >= num.length()) return false;
    
    bool foundDot = false;
    bool foundDigit = false;
    
    for (size_t i = start; i < num.length(); i++) {
        if (num[i] == '.') {
            if (foundDot) return false;
            foundDot = true;
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(num[i]))) return false;
        foundDigit = true;
    }
    return foundDigit;
}

bool ScalarConverter::matchesDoublePattern(const std::string& s) {
    if (s.empty()) return false;
    
    size_t start = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    if (start >= s.length()) return false;
    
    bool foundDot = false;
    bool foundDigit = false;
    
    for (size_t i = start; i < s.length(); i++) {
        if (s[i] == '.') {
            if (foundDot) return false;
            foundDot = true;
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) return false;
        foundDigit = true;
    }
    return foundDot && foundDigit;
}

bool ScalarConverter::matchesPseudoPattern(const std::string& s) {
    return (s == "nan" || s == "nanf" || s == "+inf" || s == "+inff" ||
            s == "-inf" || s == "-inff" || s == "inf" || s == "inff");
}

ScalarConverter::LiteralType ScalarConverter::detectType(const std::string& literal) {
    if (matchesPseudoPattern(literal)) return PSEUDO_LITERAL;
    if (matchesCharPattern(literal)) return CHAR_LITERAL;
    if (matchesFloatPattern(literal)) return FLOAT_LITERAL;
    if (matchesDoublePattern(literal)) return DOUBLE_LITERAL;
    if (matchesIntPattern(literal)) return INT_LITERAL;
    return INVALID;
}

ScalarConverter::LiteralValue ScalarConverter::parse(const std::string& literal) {
    LiteralValue result;
    result.type = detectType(literal);
    
    switch (result.type) {
        case CHAR_LITERAL:
            if (literal.length() == 1)
                result.value.c = literal[0];
            else
                result.value.c = literal[1]; // quoted form: 'a'
            break;
        case INT_LITERAL:
            result.value.i = std::atoi(literal.c_str());
            break;
        case FLOAT_LITERAL:
            result.value.f = static_cast<float>(std::atof(literal.c_str()));
            break;
        case DOUBLE_LITERAL:
            result.value.d = std::atof(literal.c_str());
            break;
        case PSEUDO_LITERAL: {
            if (literal.find("nan") != std::string::npos)
                result.value.d = std::numeric_limits<double>::quiet_NaN();
            else if (!literal.empty() && literal[0] == '-')
                result.value.d = -std::numeric_limits<double>::infinity();
            else
                result.value.d = std::numeric_limits<double>::infinity();
            break;
        }
        case INVALID:
            result.value.d = 0;
            break;
    }
    return result;
}

void ScalarConverter::printCharFromValue(double val) {
    if (std::isnan(val) || std::isinf(val)) {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    
    int intVal = static_cast<int>(val);
    if (intVal < 0 || intVal > 127) {
        std::cout << "char: impossible" << std::endl;
        return;
    }
    
    if (intVal < 32 || intVal == 127) {
        std::cout << "char: Non displayable" << std::endl;
        return;
    }
    
    std::cout << "char: '" << static_cast<char>(intVal) << "'" << std::endl;
}

void ScalarConverter::printIntFromValue(double val) {
    if (std::isnan(val) || std::isinf(val) ||
        val < static_cast<double>(std::numeric_limits<int>::min()) ||
        val > static_cast<double>(std::numeric_limits<int>::max())) {
        std::cout << "int: impossible" << std::endl;
        return;
    }
    std::cout << "int: " << static_cast<int>(val) << std::endl;
}

void ScalarConverter::printFloatFromValue(double val) {
    float fVal = static_cast<float>(val);
    std::cout << "float: " << fVal;
    
    if (std::isnan(fVal) || std::isinf(fVal)) {
        std::cout << "f" << std::endl;
        return;
    }
    
    if (fVal == static_cast<int>(fVal)) {
        std::cout << ".0f" << std::endl;
        return;
    }
    
    std::cout << "f" << std::endl;
}

void ScalarConverter::printDoubleFromValue(double val) {
    std::cout << "double: " << val;
    
    if (std::isnan(val) || std::isinf(val)) {
        std::cout << std::endl;
        return;
    }
    
    if (val == static_cast<int>(val)) {
        std::cout << ".0" << std::endl;
        return;
    }
    
    std::cout << std::endl;
}

void ScalarConverter::outputAllTypes(double val) {
    printCharFromValue(val);
    printIntFromValue(val);
    printFloatFromValue(val);
    printDoubleFromValue(val);
}

void ScalarConverter::convert(const std::string& literal) {
    if (literal.empty()) {
        std::cout << "Error: empty literal" << std::endl;
        return;
    }
    
    LiteralValue parsed = parse(literal);
    
    switch (parsed.type) {
        case CHAR_LITERAL:
            outputAllTypes(static_cast<double>(parsed.value.c));
            break;
        case INT_LITERAL:
            outputAllTypes(static_cast<double>(parsed.value.i));
            break;
        case FLOAT_LITERAL:
            outputAllTypes(static_cast<double>(parsed.value.f));
            break;
        case DOUBLE_LITERAL:
        case PSEUDO_LITERAL:
            outputAllTypes(parsed.value.d);
            break;
        case INVALID:
            std::cout << "Error: invalid literal" << std::endl;
            break;
    }
}