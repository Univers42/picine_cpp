/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PhoneBook.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 15:15:39 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/29 15:15:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE00_MYAWESOMEPHONEBOOK_PHONEBOOK_HPP_
#define CPP_MODULE00_MYAWESOMEPHONEBOOK_PHONEBOOK_HPP_

#include "./Contact.hpp"

#define MAX_CONTACT 8
class PhoneBook {
 private:  // only used inside phonebook
  Contact contact[MAX_CONTACT];
  int cap;            // capacity of contact array
  int size;           // next write index (circular)
  int count;          // number of stored contacts (<= cap)
 public:              // can be called outside phonebook
  PhoneBook();        // initialize cap and size
  void add();         // save a new contact
  void search(void);  // list/search contacts
  void exit(void);
  void seed(void);  // fill with a few default contacts at startup

  // Read-only accessors for tests / external inspection
  int getCount() const;
  const Contact& getContact(int idx) const;

  // Test helper: add a contact programmatically
  bool pushContact(const Contact& c);
};

#endif  // CPP_MODULE00_MYAWESOMEPHONEBOOK_PHONEBOOK_HPP_
