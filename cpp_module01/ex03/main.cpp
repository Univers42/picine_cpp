/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:45:41 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 05:45:42 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>

#include <iostream>
#include <string>

int main() {
    std::string greed = "HI THIS IS BRAIN";
    std::string* stringPtr = &greed;
    std::string& stringRef = greed;

    std::cout << "&greed        = " << &greed << '\n';
    std::cout << "stringPtr     = " << stringPtr << '\n';
    std::cout << "&stringRef    = " << &stringRef << '\n';

    std::cout << "greed         = " << greed << '\n';
    std::cout << "*stringPtr    = " << *stringPtr << '\n';
    std::cout << "stringRef     = " << stringRef << '\n';

    std::cout << "sizeof(std::string) = " << sizeof(greed)
              << ", sizeof(pointer) = " << sizeof(stringPtr) << '\n';
    std::cout << "greed.size() = " << greed.size() << '\n';

    std::cout << "address of internal buffer = " << static_cast<const void*>(greed.c_str()) << '\n';
    return 0;
}
