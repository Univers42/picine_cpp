#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

#include <string>
#include <iostream>

class Zombie
{
	public:
		Zombie(const std::string& name);
		void	announce() const;
		~Zombie();
	private:
		std::string name;	
};

#endif