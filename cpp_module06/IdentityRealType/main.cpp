/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 22:00:39 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 22:00:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Functions.hpp"
#include "Base.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(NULL)));
    
    std::cout << "=== Type Identification Test ===" << std::endl;
    std::cout << std::endl;
    
    // Test 1: Generate and identify multiple objects using pointers
    std::cout << "--- Test 1: Pointer Identification ---" << std::endl;
    for (int i = 0; i < 5; i++) {
        Base* obj = generate();
        std::cout << "Identified by pointer: ";
        identify(obj);
        delete obj;
        std::cout << std::endl;
    }
    
    // Test 2: Generate and identify using references
    std::cout << "--- Test 2: Reference Identification ---" << std::endl;
    for (int i = 0; i < 5; i++) {
        Base* obj = generate();
        std::cout << "Identified by reference: ";
        identify(*obj);
        delete obj;
        std::cout << std::endl;
    }
    
    // Test 3: Edge case - NULL pointer
    std::cout << "--- Test 3: NULL Pointer ---" << std::endl;
    Base* nullPtr = NULL;
    std::cout << "Identifying NULL pointer: ";
    identify(nullPtr);
    std::cout << std::endl;
    
    // Test 4: Verify both methods give same result
    std::cout << "--- Test 4: Consistency Check ---" << std::endl;
    Base* testObj = generate();
    std::cout << "Pointer method:   ";
    identify(testObj);
    std::cout << "Reference method: ";
    identify(*testObj);
    delete testObj;
    
    return 0;
}