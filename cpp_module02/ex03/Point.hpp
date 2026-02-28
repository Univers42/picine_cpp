/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Point.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 21:41:23 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 21:52:44 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POINT_HPP
# define POINT_HPP

# include "Fixed.hpp"

class Point {
private:
    Fixed const _x;
    Fixed const _y;

public:
    // Orthodox Canonical Form
    Point();
    Point(const float x, const float y);
    Point(const Point &other);
    Point &operator=(const Point &other);
    ~Point();

    // Getters
    Fixed const &getX(void) const;
    Fixed const &getY(void) const;
};

// BSP function prototype
bool bsp(Point const a, Point const b, Point const c, Point const point);

#endif
