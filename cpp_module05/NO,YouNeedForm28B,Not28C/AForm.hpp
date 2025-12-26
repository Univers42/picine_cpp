/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AForm.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:11:15 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 21:11:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AFORM_HPP
# define AFORM_HPP

# include <string>
# include <iostream>
# include <exception>

class Bureaucrat; // forward declaration

class AForm
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

	// Canonical
	AForm();
	AForm(const std::string& name, int gradeToSign, int gradeToExecute);
	AForm(const AForm& other);
	AForm& operator=(const AForm& other);
	virtual ~AForm();

	// Accessors
	std::string	getName() const;
	bool		isSigned() const;
	int			getGradeToSign() const;
	int			getGradeToExecute() const;

	// Actions
	void		beSigned(const Bureaucrat& bureaucrat);

	// Execute framework: performs common checks then calls concrete action
	void		execute(const Bureaucrat& executor) const;

protected:
	// concrete forms implement this to perform their action (assumes checks done)
	virtual void executeAction(const Bureaucrat& executor) const = 0;

private:
	const std::string _name;
	bool _isSigned;
	const int _gradeToSign;
	const int _gradeToExecute;
};

// insertion operator (non-member)
std::ostream& operator<<(std::ostream& os, const AForm& form);

#endif // AFORM_HPP