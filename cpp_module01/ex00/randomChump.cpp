#include "Zombie.hpp"

void    randomChump( const std::string& name )
{
    // create on stack (explicit)
    Zombie   zomb(name, false);
    zomb.announce();
}