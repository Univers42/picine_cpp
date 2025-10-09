#include "HumanB.hpp"
#include <iostream>

HumanB::HumanB(const std::string& name) : name(name), weapon(NULL) { }

HumanB::~HumanB(){ }

void HumanB::setWeapon( Weapon& weapon )
{
    this->weapon = &weapon;
}

void HumanB::attack( void )
{
    if (this->weapon)
        std::cout << this->name << " attacks with their " << this->weapon->getType() << std::endl;
    else
        std::cout << this->name << " has no weapon" << std::endl;
}