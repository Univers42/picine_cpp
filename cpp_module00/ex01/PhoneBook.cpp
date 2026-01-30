/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 01:07:41 by marvin            #+#    #+#             */
/*   Updated: 2026/01/29 20:51:45 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PhoneBook.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include "Contact.hpp"

/**
 * Constructor
 */
PhoneBook::PhoneBook() : cap(8), size(0), count(0) {
  // there is no additional work after reinitialization
}

/**
 * Helper to show a field as a 10-char column (truncate with '.' if needed)
 */
static std::string formatField(const std::string& s) {
  return (s.size() > 10 ? s.substr(0, 8) + "." : s);
}

// helper: return true if string is empty or contains only whitespace
static bool is_blank(const std::string& s) {
  for (std::size_t i = 0; i < s.size(); ++i)
    if (!std::isspace(static_cast<unsigned char>(s[i]))) return (false);
  return (true);
}

void PhoneBook::seed(void) {
  int loaded;
  int i;
  const char* db[][5] = {
      {"Alice", "Dupont", "Al", "0123456789", "loves cats"},
      {"Bob", "Martin", "Bobby", "+33123456789", "afraid of heights"},
      {"Carol", "Durand", "Caz", "0667788990", "secret piano"},
      {"Dave", "Petit", "D", "0789012345", "collects stamps"},
      {"Eve", "Moreau", "Ev", "0498765432", "hates spinach"}};
  const int n = sizeof(db) / sizeof(db[0]);

  loaded = (n < this->cap) ? n : this->cap;
  for (i = 0; i < loaded; ++i) {
    this->contact[i].setField(Contact::FIELD_NAME, db[i][0]);
    this->contact[i].setField(Contact::FIELD_LASTNAME, db[i][1]);
    this->contact[i].setField(Contact::FIELD_NICKNAME, db[i][2]);
    this->contact[i].setField(Contact::FIELD_PHONE, db[i][3]);
    this->contact[i].setField(Contact::FIELD_SECRET, db[i][4]);
  }
  this->count = loaded;
  this->size = loaded % this->cap;
}

void PhoneBook::search(void) {
  if (!this->cap || this->count == 0) {
    std::cout << "No contacts to search." << std::endl;
    return;
  }
  // print header
  std::cout << std::setw(10) << "index" << "|" << std::setw(10) << "first_name"
            << "|" << std::setw(10) << "last_name" << "|" << std::setw(10)
            << "nickname" << std::endl;
  for (int i = 0; i < this->count; ++i) {
    std::string fn =
        formatField(this->contact[i].getField(Contact::FIELD_NAME));
    std::string ln =
        formatField(this->contact[i].getField(Contact::FIELD_LASTNAME));
    std::string nn =
        formatField(this->contact[i].getField(Contact::FIELD_NICKNAME));
    std::cout << std::setw(10) << i << "|" << std::setw(10) << fn << "|"
              << std::setw(10) << ln << "|" << std::setw(10) << nn << std::endl;
  }

  // Prompt repeatedly for index after showing all contacts; empty line cancels
  std::string input;
  while (true) {
    std::cout << "Enter index to display (empty to cancel): ";
    if (!std::getline(std::cin, input)) {
      std::cout << "Aborted." << std::endl;
      return;
    }
    // trim
    size_t start = input.find_first_not_of(" \t\n\r\v\f");
    if (start == std::string::npos) {
      // empty -> cancel
      return;
    }
    size_t end = input.find_last_not_of(" \t\n\r\v\f");
    input = input.substr(start, end - start + 1);

    // check numeric
    bool ok = !input.empty();
    for (std::size_t k = 0; k < input.size() && ok; ++k)
      if (!std::isdigit(static_cast<unsigned char>(input[k]))) ok = false;
    if (!ok) {
      std::cout << "Invalid index." << std::endl;
      continue;
    }

    int idx = std::atoi(input.c_str());
    if (idx < 0 || idx >= this->count) {
      std::cout << "Invalid index." << std::endl;
      continue;
    }

    // Display all fields of the selected contact, one per line
    std::cout << "First Name: "
              << this->contact[idx].getField(Contact::FIELD_NAME) << std::endl;
    std::cout << "Last Name: "
              << this->contact[idx].getField(Contact::FIELD_LASTNAME)
              << std::endl;
    std::cout << "Nickname: "
              << this->contact[idx].getField(Contact::FIELD_NICKNAME)
              << std::endl;
    std::cout << "Phone: " << this->contact[idx].getField(Contact::FIELD_PHONE)
              << std::endl;
    std::cout << "Secret: "
              << this->contact[idx].getField(Contact::FIELD_SECRET)
              << std::endl;
    return;
  }
}

void PhoneBook::add(void) {
  std::string name;
  std::string lastName;
  std::string nickName;
  std::string phoneNumber;
  std::string darkestSecret;

  // Read required fields; if any read fails or is empty/blank, abort add.
  std::cout << "Enter the name: ";
  if (!std::getline(std::cin, name) || is_blank(name)) return;
  std::cout << "Enter the last name: ";
  if (!std::getline(std::cin, lastName) || is_blank(lastName)) return;
  std::cout << "Enter the nick name: ";
  if (!std::getline(std::cin, nickName) || is_blank(nickName)) return;
  std::cout << "Enter the phone number: ";
  if (!std::getline(std::cin, phoneNumber) || is_blank(phoneNumber)) return;
  std::cout << "Enter the darkest secret: ";
  if (!std::getline(std::cin, darkestSecret) || is_blank(darkestSecret)) return;

  this->contact[size].setField(Contact::FIELD_NAME, name);
  this->contact[size].setField(Contact::FIELD_LASTNAME, lastName);
  this->contact[size].setField(Contact::FIELD_NICKNAME, nickName);
  this->contact[size].setField(Contact::FIELD_PHONE, phoneNumber);
  this->contact[size].setField(Contact::FIELD_SECRET, darkestSecret);
  this->size = (this->size + 1) % this->cap;
  if (this->count < this->cap) this->count++;
}

// little trick here to quit carefully we need to call the namespace std
// to avoid infinite recursion
void PhoneBook::exit(void) { std::exit(EXIT_SUCCESS); }

int PhoneBook::getCount() const { return (this->count); }

const Contact& PhoneBook::getContact(int idx) const {
  if (this->count == 0) {
    static Contact dummy;
    return dummy;
  }
  if (idx < 0) {
    return this->contact[0];
  }
  if (idx >= this->count) {
    return this->contact[this->count - 1];
  }
  return this->contact[idx];
}

// Insert a contact programmatically; returns false if any required field empty
bool PhoneBook::pushContact(const Contact& c) {
  const std::string& n = c.getField(Contact::FIELD_NAME);
  const std::string& ln = c.getField(Contact::FIELD_LASTNAME);
  const std::string& nn = c.getField(Contact::FIELD_NICKNAME);
  const std::string& ph = c.getField(Contact::FIELD_PHONE);
  const std::string& sec = c.getField(Contact::FIELD_SECRET);

  if (is_blank(n) || is_blank(ln) || is_blank(nn) || is_blank(ph) ||
      is_blank(sec))
    return false;
  this->contact[this->size] = c;
  this->size = (this->size + 1) % this->cap;
  if (this->count < this->cap) this->count++;
  return true;
}
