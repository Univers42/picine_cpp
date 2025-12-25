#ifndef ZOMBIE_HPP
# define ZOMBIE_HPP

#include <iostream>
#include <string>

class Zombie
{
	public:
		// nicer constructor: second param indicates whether this zombie was allocated on the heap
		Zombie(const std::string &name, bool heap = false);
		void	announce( void );
		void	setName( const std::string &name);
		~Zombie();

		// ANSI color helpers - declared (definitions moved to .cpp for C++98 compatibility)
		static const char* C_RESET;
		static const char* C_RED;
		static const char* C_GREEN;
		static const char* C_YELLOW;
		static const char* C_CYAN;
	private:
		std::string name;
		bool allocatedOnHeap;
};
#endif
