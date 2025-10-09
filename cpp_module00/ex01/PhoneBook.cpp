#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "PhoneBook.hpp"

/**
 * Constructor 
 */
PhoneBook::PhoneBook() : cap(8), size(0), count(0)
{
	// there is no additional work after reinitialization
}

/**
 * Helper to show a field as a 10-char column (truncate with '.' if needed)
 */
static std::string	formatField(const std::string &s)
{
	if (s.size() > 10)
		return s.substr(0, 9) + ".";
	return s;
}

void	PhoneBook::seed(void)
{
	int			loaded;
	int			i;
	const char *db[][5] =
	{
		{"Alice", "Dupont", "Al", "0123456789", "loves cats"},
		{"Bob", "Martin", "Bobby", "+33123456789", "afraid of heights"},
		{"Carol", "Durand", "Caz", "0667788990", "secret piano"},
		{"Dave", "Petit", "D", "0789012345", "collects stamps"},
		{"Eve", "Moreau", "Ev", "0498765432", "hates spinach"}
	};
	const int n = sizeof(db) / sizeof(db[0]);
	loaded = (n < this->cap) ? n : this->cap;
	i = 0;
	while (++i < loaded)
	{
		this->contact[i].setField(Contact::SLOT_Name, db[i][0]);
		this->contact[i].setField(Contact::SLOT_LASTNAME, db[i][1]);
		this->contact[i].setField(Contact::SLOT_NICKNAME, db[i][2]);
		this->contact[i].setField(Contact::SLOT_PHONE, db[i][3]);
		this->contact[i].setField(Contact::SLOT_SECRET, db[i][4]);
	}
	this->count = loaded;
	this->size = loaded % this->cap;
}

void	PhoneBook::search(void)
{
	// print header
	std::cout << std::setw(10) << "index" << "|"
			  << std::setw(10) << "first_name" << "|"
			  << std::setw(10) << "last_name" << "|"
			  << std::setw(10) << "nick_name" << std::endl;
	// print rows
	for (int i = 0; i < this->count; ++i)
	{
		std::string fn = formatField(this->contact[i].getField(Contact::SLOT_Name));
		std::string ln = formatField(this->contact[i].getField(Contact::SLOT_LASTNAME));
		std::string nn = formatField(this->contact[i].getField(Contact::SLOT_NICKNAME));
		std::cout << std::setw(10) << i << "|"
				  << std::setw(10) << fn << "|"
				  << std::setw(10) << ln << "|"
				  << std::setw(10) << nn << std::endl;
	}
}

void	PhoneBook::add(void)
{
	std::string	name;
	std::string lastName;
	std::string nickName;
	std::string phoneNumber;
	std::string	darkestSecret;

	// Read required fields; if any read fails or is empty, abort add.
	std::cout << "Enter the name: ";
	if (!std::getline(std::cin, name) || name.empty())
		return;
	std::cout << "Enter the last name: ";
	if (!std::getline(std::cin, lastName) || lastName.empty())
		return;
	std::cout << "Enter the nick name: ";
	if (!std::getline(std::cin, nickName) || nickName.empty())
		return;
	std::cout << "Enter the phone number: ";
	if (!std::getline(std::cin, phoneNumber) || phoneNumber.empty())
		return;
	std::cout << "Enter the darkest secret: ";
	if (!std::getline(std::cin, darkestSecret) || darkestSecret.empty())
		return ;
	// Store fields using enum slots
	this->contact[size].setField(Contact::SLOT_Name, name);
	this->contact[size].setField(Contact::SLOT_LASTNAME, lastName);
	this->contact[size].setField(Contact::SLOT_NICKNAME, nickName);
	this->contact[size].setField(Contact::SLOT_PHONE, phoneNumber);
	this->contact[size].setField(Contact::SLOT_SECRET, darkestSecret);
	// advance size with wrap-around
	this->size = (this->size + 1) % this->cap;
	if (this->count < this->cap)
		this->count++;
}

// little trick here to quit carefully we need to call the namespace std
// to avoid infinite recursion
void    PhoneBook::exit(void)
{
	std::exit(EXIT_SUCCESS);
}