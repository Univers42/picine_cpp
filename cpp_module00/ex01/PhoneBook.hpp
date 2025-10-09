#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

#include "Contact.hpp"

# define MAX_CONTACT 8
class PhoneBook
{
	private:	//only used inside phonebook
		Contact	contact[MAX_CONTACT];
		int		cap;		//capacity of contact array
		int		size;    // next write index (circular)
		int		count;   // number of stored contacts (<= cap)
	public:		//can be called outside phonebook
		PhoneBook(); // initialize cap and size
		void add();	//save a new contact
		void search(void); // list/search contacts
		void exit(void);
		void seed(void); // fill with a few default contacts at startup
};

#endif // PHONEBOOK_HPP