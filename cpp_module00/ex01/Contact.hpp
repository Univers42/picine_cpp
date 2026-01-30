/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Contact.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 19:58:06 by marvin            #+#    #+#             */
/*   Updated: 2026/01/29 15:15:48 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPP_MODULE00_MYAWESOMEPHONEBOOK_CONTACT_HPP_
#define CPP_MODULE00_MYAWESOMEPHONEBOOK_CONTACT_HPP_

#include <string>

class Contact {
 public:
  enum Field {
    FIELD_NULL = 0,
    FIELD_NAME,
    FIELD_LASTNAME,
    FIELD_NICKNAME,
    FIELD_PHONE,
    FIELD_SECRET,
    FIELD_COUNT
  };
  Contact();
  void setField(Field f, const std::string& v);
  const std::string getField(Field f) const;

 private:
  int idxField;
  std::string fields[FIELD_COUNT];
  static const std::string& emptyString();
};

#endif  // CPP_MODULE00_MYAWESOMEPHONEBOOK_CONTACT_HPP_
