#ifndef HARL_HPP
# define HARL_HPP

# include <string>

class   Harl
{
	public:
		typedef void (Harl::*FuncPtr)(void);
		enum { LVL_DEBUG, LVL_INFO, LVL_WARNING, LVL_ERROR, LVL_COUNT };
		Harl();
		~Harl();
		void	complain( std::string level );
	private:
		void	debug( void );
		void	info( void );
		void	warning( void );
		void	error( void );
};
#endif