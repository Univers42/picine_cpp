#include "Contact.hpp"
#include <algorithm>
#include <cctype>

const std::string &Contact::emptyString()
{
	static std::string s;
	return (s);
}

Contact::Contact()
	: idxField(0)
{
	// initialize fields to empty strings (std::string default ctor already does that)
}

void Contact::setField(Field f, const std::string &v)
{
	int idx = static_cast<int>(f);
	// Only accept valid defined slots (1..SLOT_COUNT-1). Ignore SLOT_NULL.
	if (idx > 0 && idx < static_cast<int>(SLOT_COUNT))
		this->fields[idx] = v;
	// else ignore invalid field or SLOT_NULL
}

const std::string Contact::getField(Field f) const
{
	int idx = static_cast<int>(f);
	if (idx >= 0 && idx < static_cast<int>(SLOT_COUNT))
		return this->fields[idx];
	return Contact::emptyString();
}
