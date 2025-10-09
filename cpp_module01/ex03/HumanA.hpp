#ifndef HUMANA_HPP
# define HUMANA_HPP

#include "Weapon.hpp"
#include <string>

class   HumanA
{
    private:
        std::string name;
        Weapon&     weapon; // store by reference so it always reflects changes
    public:
        HumanA( const std::string&name, Weapon& weapon );
        ~HumanA( void );
        void    attack();
};

#endif