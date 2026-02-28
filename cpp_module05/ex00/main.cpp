#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include <iostream>

int main()
{
	Intern intern;
	Bureaucrat john("John", 140); // can sign shrubbery (145) but not robotomy (72) or pardon (25)
	Bureaucrat boss("Boss", 1);   // can sign/execute everything

	std::cout << "=== Intern test: create shrubbery ===\n";
	AForm* f1 = intern.makeForm("shrubbery creation", "home");
	if (f1) {
		std::cout << *f1 << '\n';
		john.signForm(*f1);    // should succeed
		john.executeForm(*f1); // should succeed (exec 137, john 140 -> actually john cannot execute; boss will)
		boss.executeForm(*f1); // boss executes
		delete f1;
	}

	std::cout << "\n=== Intern test: create robotomy ===\n";
	AForm* f2 = intern.makeForm("robotomy request", "Bender");
	if (f2) {
		std::cout << *f2 << '\n';
		john.signForm(*f2);     // should fail (john grade too low)
		boss.signForm(*f2);     // boss signs
		boss.executeForm(*f2);  // boss executes (50% success)
		delete f2;
	}

	std::cout << "\n=== Intern test: create presidential pardon ===\n";
	AForm* f3 = intern.makeForm("presidential pardon", "Arthur Dent");
	if (f3) {
		std::cout << *f3 << '\n';
		john.signForm(*f3);     // should fail
		boss.signForm(*f3);     // boss signs
		boss.executeForm(*f3);  // boss executes
		delete f3;
	}

	std::cout << "\n=== Intern test: unknown form ===\n";
	AForm* f4 = intern.makeForm("make coffee", "kitchen");
	if (f4) {
		// shouldn't happen, but be safe
		delete f4;
	}

	return 0;
}
