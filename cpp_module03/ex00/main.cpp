/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:55:36 by dlesieur          #+#    #+#             */
/*   Updated: 2025/11/13 15:57:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClapTrap.hpp"

int main()
{
    ClapTrap a("alpha");
    ClapTrap b = a;
    ClapTrap c = std::move(a);   //moved
    b = c;                      //assigned(copy)
    c = ClapTrap("alpha");            // assigned (move)
    return (0);   
}