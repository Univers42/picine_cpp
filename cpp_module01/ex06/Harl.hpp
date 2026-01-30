/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Harl.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 05:44:54 by dlesieur          #+#    #+#             */
/*   Updated: 2026/01/30 06:43:37 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __HARL_HPP__
# define __HARL_HPP__

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

#endif // __HARL_HPP__
