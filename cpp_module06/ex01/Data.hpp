/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/25 21:55:30 by marvin            #+#    #+#             */
/*   Updated: 2025/12/25 21:55:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
#define DATA_HPP

#include <string>

struct Data {
    int id;
    double value;
    std::string name;
    
    Data() : id(0), value(0.0), name("") {}
    Data(int i, double v, std::string n) : id(i), value(v), name(n) {}
};

#endif