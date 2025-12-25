/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bureaucrat.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 19:37:22 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 19:37:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP

#include <string>
#include <iostream>
#include <exception>

/**
 * Invariant enforcement:
 * A Bureaucrat's grade must ALWAYS be between 1 and 150.
 * Any attempt to violate this invariant throws an exception.
 */
class Bureaucrat
{
public:
    // Exceptions
    class GradeTooHighException : public std::exception {
    public:
        const char* what() const noexcept override;
    };

    class GradeTooLowException : public std::exception {
    public:
        const char* what() const noexcept override;
    };

    // Orthodox Canonical Form
    Bureaucrat();                                    // Default constructor
    Bureaucrat(const Bureaucrat& other);             // Copy constructor
    Bureaucrat& operator=(const Bureaucrat& other);  // Copy assignment
    ~Bureaucrat();                                   // Destructor

    // Parametrized constructor
    Bureaucrat(const std::string& name, int grade);

    // Accessors
    std::string getName() const;
    int getGrade() const;

    // Modifiers
    void incrementGrade();
    void decrementGrade();

private:
    const std::string _name;
    int _grade;

    void validateGrade(int grade) const;
};

std::ostream& operator<<(std::ostream& os, const Bureaucrat& b);

#endif // BUREAUCRAT_HPP
