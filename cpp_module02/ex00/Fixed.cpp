#include "Fixed.hpp"
#include <iostream>

Fixed::Fixed() : fixedPoint(0)
{
    std::cout << "Default constructor called\n";
}

Fixed::Fixed(const Fixed &other)
{
    std::cout << "Copy constructor called\n";
    *this = other;
}

Fixed  &Fixed::operator=(const Fixed &other)
{
    std::cout << "Copy assignment operator called\n";
    if (this != &other)
        this->fixedPoint = other.getRawBits();
    return *this;
}

Fixed::~Fixed()
{
    std::cout << "Destructor called\n";
}

int Fixed::getRawBits() const
{
    return (fixedPoint);
}

void Fixed::setRawBits(int const raw)
{
    fixedPoint = raw;
}