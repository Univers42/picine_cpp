/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bsp.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 21:48:43 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 18:06:21 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Point.hpp"

/* Calculates the Z-component of the cross product of two vectors (p1->p3 and
 * p2->p3).
 * Formula: (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3)
 */
static Fixed crossProduct(Point const p1, Point const p2, Point const p3) {
  return (p1.getX() - p3.getX()) * (p2.getY() - p3.getY()) -
         (p2.getX() - p3.getX()) * (p1.getY() - p3.getY());
}

bool bsp(Point const a, Point const b, Point const c, Point const point) {
  Fixed d1, d2, d3;
  bool has_neg, has_pos;

  // Calculate cross products for the 3 edges
  d1 = crossProduct(point, a, b);
  d2 = crossProduct(point, b, c);
  d3 = crossProduct(point, c, a);

  // Subject requirement: If point is a vertex or on an edge, return False.
  // An edge collision yields a cross product of exactly 0.
  if (d1 == Fixed(0) || d2 == Fixed(0) || d3 == Fixed(0)) return false;

  // Check if the signs are mixed
  has_neg = (d1 < Fixed(0)) || (d2 < Fixed(0)) || (d3 < Fixed(0));
  has_pos = (d1 > Fixed(0)) || (d2 > Fixed(0)) || (d3 > Fixed(0));

  // If it has BOTH negative and positive cross products, the point is OUTSIDE.
  // If it is entirely negative OR entirely positive, it is strictly INSIDE.
  return !(has_neg && has_pos);
}
