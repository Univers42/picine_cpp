/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:37:29 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 19:37:29 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main() {
    std::cout << "=== Test 1: basic sign flow ===" << std::endl;
    try {
        Bureaucrat alice("Alice", 3);
        Form f("TopSecret", 2, 5);
        std::cout << alice << std::endl;
        std::cout << f << std::endl;
        alice.signForm(f); // should fail
        alice.incrementGrade(); // 2
        alice.signForm(f); // should succeed
        std::cout << f << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== Test 2: low-grade bureaucrat ===" << std::endl;
    try {
        Bureaucrat bob("Bob", 150);
        Form easy("EasyForm", 149, 150);
        std::cout << bob << std::endl;
        std::cout << easy << std::endl;
        bob.signForm(easy); // should fail
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== Test 3: top bureaucrat signs everything ===" << std::endl;
    try {
        Bureaucrat carl("Carl", 1);
        Form hard("SecretMission", 1, 1);
        std::cout << carl << std::endl;
        std::cout << hard << std::endl;
        carl.signForm(hard); // should succeed
        std::cout << hard << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== Test 4: invalid form creation (should throw) ===" << std::endl;
    try {
        Form bad("BadForm", 0, 10); // invalid: grade 0
    } catch (std::exception& e) {
        std::cerr << "Caught expected exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== Test 5: copy form and signing ===" << std::endl;
    try {
        Bureaucrat dave("Dave", 5);
        Form original("Original", 5, 10);
        Form copy = original;
        std::cout << "Before signing: " << original << " | copy: " << copy << std::endl;
        dave.signForm(copy); // should succeed
        std::cout << "After signing copy: original: " << original << " | copy: " << copy << std::endl;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}