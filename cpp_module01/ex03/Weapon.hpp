// ...existing code...
#ifndef WEAPON_HPP
# define WEAPON_HPP
#include <string>

class Weapon
{
    public:
        Weapon(const std::string& type);
        ~Weapon();
        const std::string&	getType( void ) const;
        void				setType( const std::string& v );
    private:
        std::string	type;
};
#endif