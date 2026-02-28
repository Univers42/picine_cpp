#include "Harl.hpp"

int main(void) {
    Harl harl;

    std::cout << "--- Harl is hungry ---" << std::endl;
    harl.complain("DEBUG");

    std::cout << "\n--- Harl is curious ---" << std::endl;
    harl.complain("INFO");

    std::cout << "\n--- Harl is annoyed ---" << std::endl;
    harl.complain("WARNING");

    std::cout << "\n--- Harl is furious ---" << std::endl;
    harl.complain("ERROR");

    std::cout << "\n--- Harl is confused (Unknown level) ---" << std::endl;
    harl.complain("MCDONALDS");

    return 0;
}
