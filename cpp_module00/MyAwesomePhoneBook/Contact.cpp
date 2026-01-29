/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 20:05:02 by marvin            #+#    #+#             */
/*   Updated: 2026/01/29 00:44:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Contact.hpp"
#include <algorithm>
#include <cctype>

const std::string& Contact::emptyString() {
  static std::string s;

  return (s);
}

Contact::Contact() : idxField(0) {}

void Contact::setField(Field f, const std::string& v) {
  int idx = static_cast<int>(f);
  if (idx > 0 && idx < static_cast<int>(FIELD_COUNT)) this->fields[idx] = v;
}

const std::string Contact::getField(Field f) const {
  int idx = static_cast<int>(f);

  if (idx >= 0 && idx < static_cast<int>(FIELD_COUNT))
    return (this->fields[idx]);
  return (Contact::emptyString());
}
