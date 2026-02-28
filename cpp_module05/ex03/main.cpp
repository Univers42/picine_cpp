#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "Bureaucrat.hpp"
#include <iostream>

int main()
{
	// actors
	Bureaucrat alice("Alice", 50);   // mid grade
	Bureaucrat bob("Bob", 1);        // top grade
	Bureaucrat tom("Tom", 150);      // low grade

	// forms
	ShrubberyCreationForm shrub("home");
	RobotomyRequestForm robo("Bender");
	PresidentialPardonForm pardon("Ford");

	std::cout << "=== Initial state ===\n";
	std::cout << shrub << '\n' << robo << '\n' << pardon << '\n';

	std::cout << "\n=== Signing attempts ===\n";
	alice.signForm(shrub);   // should succeed (sign <=145)
	alice.signForm(robo);    // should succeed (sign <=72)
	alice.signForm(pardon);  // should fail (sign 25 required)

	std::cout << "\n=== Execute attempts ===\n";
	// Shrubbery: exec 137, alice(50) can execute
	alice.executeForm(shrub);

	// Robotomy: exec 45, alice(50) cannot; bob(1) can
	alice.executeForm(robo); // expected failure due to grade
	bob.executeForm(robo);   // success or random failure (50%)

	// Presidential: exec 5, bob can execute after signing
	bob.signForm(pardon);    // bob signs
	bob.executeForm(pardon); // bob executes

	std::cout << "\n=== Edge cases ===\n";
	// try executing an unsigned form
	ShrubberyCreationForm garden("garden");
	bob.executeForm(garden); // should report cannot execute because not signed

	// attempt sign with too low grade
	tom.signForm(robo); // should fail

	return 0;
}
