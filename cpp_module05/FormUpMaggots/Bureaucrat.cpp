/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bureaucrat.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:37:25 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 19:37:25 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bureaucrat.hpp"
#include "Form.hpp"

const char *Bureaucrat::GradeTooHighException::what() const noexcept
{
	return ("Grade is too high. Maximum grade is 150.");
}

const char *Bureaucrat::GradeTooLowException::what() const noexcept
{
	return ("Grade is too low. Minimum grade is 1.");
}

/**
 * Cannonical Form Implementations
 */

Bureaucrat::Bureaucrat() : _name("Default") , _grade(150) {}

Bureaucrat& Bureaucrat::operator=(const Bureaucrat& other)
{
	if (this != &other)
	{
		_grade = other._grade;
	}
	return (*this);
}

Bureaucrat::~Bureaucrat() {}

/**
 * Constructor
 */
Bureaucrat::Bureaucrat(const std::string& name, int grade)
	: _name(name), _grade(grade)
{
	validateGrade(grade);
}

/**
 * Accessors
 */

std::string Bureaucrat::getName() const
{
	return _name;
}

int Bureaucrat::getGrade() const
{
	return _grade;
}

/**
 * Modifiers
 */

void Bureaucrat::incrementGrade()
{
	validateGrade(_grade - 1);
	--_grade;
}

void Bureaucrat::decrementGrade()
{
	validateGrade(_grade + 1);
	++_grade;
}

/**
 * Private Methods
 */

void Bureaucrat::validateGrade(int grade) const
{
	if (grade < 1)
		throw GradeTooLowException();
	if (grade > 150)
		throw GradeTooHighException();
}

void Bureaucrat::signForm(Form& form)
{
	try {
		form.beSigned(*this);
		std::cout << _name << " signed " << form.getName() << std::endl;
	} catch (std::exception& e) {
		std::cout << _name << " couldn't sign " << form.getName()
				  << " because " << e.what() << std::endl;
	}
}

std::ostream& operator<<(std::ostream& os, const Bureaucrat& b)
{
	os << b.getName() << ", bureaucrat grade " << b.getGrade();
	return os;
}