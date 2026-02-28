/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Function.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:01:11 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 22:01:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Functions.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <cstdlib>
#include <iostream>
#include <exception>

Base* generate(void) {
    int random = rand() % 3;
    
    switch (random) {
        case 0:
            std::cout << "[Generated type A]" << std::endl;
            return new A();
        case 1:
            std::cout << "[Generated type B]" << std::endl;
            return new B();
        default:
            std::cout << "[Generated type C]" << std::endl;
            return new C();
    }
}

// ==================== Pointer Version ====================
// Template helper to check type and print - avoids code duplication
template <typename DerivedType>
bool checkAndPrint(Base* p, const char* typeName) {
    if (dynamic_cast<DerivedType*>(p)) {
        std::cout << typeName << std::endl;
        return true;
    }
    return false;
}

void identify(Base* p) {
    if (!p) {
        std::cout << "NULL pointer" << std::endl;
        return;
    }

    // try each type and return on first match
    if (checkAndPrint<A>(p, "A")) return;
    if (checkAndPrint<B>(p, "B")) return;
    if (checkAndPrint<C>(p, "C")) return;

    // fallback when none match
    std::cout << "Unknown type" << std::endl;
}

// ==================== Reference Version ====================
// Template helper for reference type checking using try-catch
// This version doesn't use pointers internally as required
template <typename DerivedType>
bool checkAndPrintRef(Base& p, const char* typeName) {
    try {
        // dynamic_cast with reference throws bad_cast on failure
        // We don't need to store the result, just checking if cast succeeds
        (void)dynamic_cast<DerivedType&>(p);
        std::cout << typeName << std::endl;
        return true;
    } catch (const std::bad_cast&) {
        return false;
    }
}

void identify(Base& p) {
    // try each type and return on first successful cast
    if (checkAndPrintRef<A>(p, "A")) return;
    if (checkAndPrintRef<B>(p, "B")) return;
    if (checkAndPrintRef<C>(p, "C")) return;

    // fallback when none match
    std::cout << "Unknown type" << std::endl;
}