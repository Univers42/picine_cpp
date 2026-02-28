#include "Intern.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <functional>
#include <iostream>

Intern::Intern() {}
Intern::Intern(const Intern& other) { (void)other; }
Intern& Intern::operator=(const Intern& other) { (void)other; return *this; }
Intern::~Intern() {}

AForm* Intern::makeForm(const std::string& formName, const std::string& target) const
{
	struct Entry { const char* name; std::function<AForm*(const std::string&)> ctor; };
	static const Entry table[] = {
		{ "shrubbery creation", [](const std::string& t){ return new ShrubberyCreationForm(t); } },
		{ "robotomy request",   [](const std::string& t){ return new RobotomyRequestForm(t); } },
		{ "presidential pardon",[](const std::string& t){ return new PresidentialPardonForm(t); } }
	};

	for (const auto& e : table) {
		if (formName == e.name) {
			AForm* f = e.ctor(target);
			std::cout << "Intern creates " << e.name << std::endl;
			return f;
		}
	}
	std::cout << "Intern: unknown form \"" << formName << "\"" << std::endl;
	return nullptr;
}
