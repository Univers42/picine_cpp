# include "Harl.hpp"
# include <iostream>
# include <string>

Harl::Harl() { }
Harl::~Harl() { }

void Harl::debug( void )
{
	std::cout << "I love having extra bacon for my 7XL-double-cheese-triple-pickle-special-ketchup burger. I really do!" << std::endl;
}

void Harl::warning( void )
{
	std::cout << "I think I deserve to have some extra bacon for free. I’ve been coming for years, whereas you started working here just last month." << std::endl;
}

void Harl::info( void )
{
	std::cout << "I cannot believe adding extra bacon costs more money. You didn’t put enough bacon in my burger! If you did, I wouldn’t be asking for more!" << std::endl;
}

void Harl::error( void )
{
	std::cout << "This is unacceptable! I want to speak to the manager now." << std::endl;
}

void Harl::complain( std::string level )
{
	static const char* names[LVL_COUNT] = { "DEBUG", "INFO", "WARNING", "ERROR" };
	static const FuncPtr funcs[LVL_COUNT] = {
		&Harl::debug, &Harl::info, &Harl::warning, &Harl::error
	};
	int	idx;

	if (level.empty())
		return;
	idx = -1;
	switch (level[0])
	{
		case 'D': idx = LVL_DEBUG; break;
		case 'I': idx = LVL_INFO; break;
		case 'W': idx = LVL_WARNING; break;
		case 'E': idx = LVL_ERROR; break;
		default: return;
	}
	// Verify exact match (guards against accidental first-letter collision)
	if (level == names[idx])
		(this->*funcs[idx])();
}