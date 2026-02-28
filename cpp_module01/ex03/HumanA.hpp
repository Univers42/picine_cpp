/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HumanA.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 16:41:20 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 16:58:15 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HUMANA_HPP
# define HUMANA_HPP

# include <string>
# include "Weapon.hpp"

class HumanA {
  private:
    std::string name;
    Weapon& weapon; // Reference because HumanA is ALWAYS armed

  public:
    HumanA(const std::string& name, Weapon& weapon);
    ~HumanA();
    void attack() const;
};

#endif
