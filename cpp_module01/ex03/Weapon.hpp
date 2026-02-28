/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Weapon.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:37:31 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 16:57:56 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEAPON_HPP
# define WEAPON_HPP

# include <string>

class Weapon {
  private:
    std::string type;

  public:
    Weapon(const std::string& type);
    ~Weapon();

    // Returns a constant reference to type (O(1) memory address return)
    const std::string& getType() const;
    void setType(const std::string& type);
};

#endif
