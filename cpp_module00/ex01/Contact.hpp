#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>

class Contact
{
	public:
		enum Field {SLOT_NULL = 0, SLOT_Name, SLOT_LASTNAME, SLOT_NICKNAME, SLOT_PHONE, SLOT_SECRET, SLOT_COUNT};
		Contact();
		void						setField(Field f, const std::string &v);
        const std::string			getField(Field f) const;
    private:
		int							idxField;
		std::string					fields[SLOT_COUNT];
		static const std::string	&emptyString();
};

#endif // CONTACT_HPP