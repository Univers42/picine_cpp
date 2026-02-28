/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:11:06 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 21:11:06 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AForm.hpp"
#include "Bureaucrat.hpp"
#include <stdexcept>

const char* AForm::GradeTooHighException::what() const noexcept
{
	return "Form grade is too high. Minimum allowed is 1.";
}

const char* AForm::GradeTooLowException::what() const noexcept
{
	return "Form grade is too low. Maximum allowed is 150.";
}

/**
 * Canonical AForm Implementations
 */
AForm::AForm()
	: _name("Default"), _isSigned(false), _gradeToSign(150), _gradeToExecute(150) {}

AForm::AForm(const std::string& name, int gradeToSign, int gradeToExecute)
	: _name(name), _isSigned(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
	if (gradeToSign < 1 || gradeToExecute < 1)
		throw GradeTooHighException();
	if (gradeToSign > 150 || gradeToExecute > 150)
		throw GradeTooLowException();
}

AForm::AForm(const AForm& other)
	: _name(other._name), _isSigned(other._isSigned),
	  _gradeToSign(other._gradeToSign), _gradeToExecute(other._gradeToExecute) {}

AForm& AForm::operator=(const AForm& other)
{
	if (this != &other)
		_isSigned = other._isSigned;
	return *this;
}

AForm::~AForm() {}

/**
 * Accessors
 */

std::string AForm::getName() const { return _name; }
bool AForm::isSigned() const { return _isSigned; }
int AForm::getGradeToSign() const { return _gradeToSign; }
int AForm::getGradeToExecute() const { return _gradeToExecute; }

void AForm::beSigned(const Bureaucrat& bureaucrat)
{
	// Bureaucrat with lower numeric grade has higher rank (1 highest)
	if (bureaucrat.getGrade() > _gradeToSign)
		throw GradeTooLowException();
	_isSigned = true;
}

void AForm::execute(const Bureaucrat& executor) const
{
	if (!isSigned())
		throw std::runtime_error("Form is not signed");
	if (executor.getGrade() > getGradeToExecute())
		throw GradeTooLowException();
	// delegate actual behaviour to concrete form
	executeAction(executor);
}

std::ostream& operator<<(std::ostream& os, const AForm& form)
{
	os << "Form \"" << form.getName() << "\" | signed: " << (form.isSigned() ? "yes" : "no")
	   << " | grade to sign: " << form.getGradeToSign()
	   << " | grade to execute: " << form.getGradeToExecute();
	return os;
}