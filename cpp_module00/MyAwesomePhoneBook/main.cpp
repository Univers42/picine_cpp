#include "PhoneBook.hpp"
#include <iostream>
#include <string>

int main( void )
{
	std::string input;
	PhoneBook pb;

    pb.seed();
    while (true)
    {
        std::cout << "$> " << std::flush;
        if (!std::getline(std::cin, input))
            break;
        if (input == "add")
            pb.add();
        else if (input == "search")
            pb.search();
        else if (input == "exit")
            break;
    }
    return 0;
}