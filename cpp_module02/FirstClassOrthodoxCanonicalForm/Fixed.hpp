#ifndef FIXED_HPP
# define FIXED_HPP

/**
 * This design pattern is used for any class that owns
 * resources (heap memory, file handles, etc.).
 * If the class only contains plain members (ints, float), the
 * defaults are often sufficient, but implementing the canonical
 * form is a good exercise and required by the piscine exercises.
 * 
 * Objective :
 * Ensure correct copy/assignment semantics (deep vs shallow copy)
 * when the class manages resources.
 * Make object lifetime predictable and explicit.
 * Follow idiomatic patterns (copy ctor takes const T&, operator= returns T& and)
 * handles self-assignment.
 * 
 * `ctor` is a short for constructor
 */
class Fixed
{
    private:
        int fixedPoint;
        static const int fractBit = 8;
    public:
        Fixed();
        Fixed(const Fixed &other);
        Fixed &operator=(const Fixed &other);
        ~Fixed();
        int getRawBits() const;
        void setRawBits(int const raw);
};
#endif