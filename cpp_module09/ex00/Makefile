# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/24 03:00:00 by marvin            #+#    #+#              #
#    Updated: 2026/03/01 13:44:28 by dlesieur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= btc

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
INC			= -I.

SRCS		= main.cpp BitcoinExchange.cpp vendor/ft_string.cpp
OBJS		= $(SRCS:.cpp=.o)

# ━━━━ Default: classic 42 build ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o:		%.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# ━━━━ MySQLite: rich table rendering build ━━━━━━━━━━━━━━━━━━━━━━━━━━━

sqlite:		fclean
	$(MAKE) CXXFLAGS="$(CXXFLAGS) -DHAVE_MY_SQL_LITE=1" LDFLAGS="-lreadline" all

# ━━━━ Cleanup ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re sqlite
