/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClapTrap.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:41:50 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/13 23:05:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLAPTRAP_HPP
# define CLAPTRAP_HPP

#include <iostream>

class ClapTrap
{
    private:
        const std::string	name;
        uint32_t			hit;
        uint32_t			nrj;
        uint32_t			attak;

    public:
        ClapTrap(const std::string& name);			// default ctor
        ~ClapTrap();								// dtor
        ClapTrap(const ClapTrap& other);			// copy ctor
        ClapTrap& operator=(const ClapTrap& other);	// copy assignment
        ClapTrap(ClapTrap&& other);					// move ctor
        ClapTrap& operator=(ClapTrap&& other);		// move assignment
        void attack(const std::string& target);
        void takeDamage(uint32_t amount);
        void beRepaired(uint32_t amount);
};
#endif