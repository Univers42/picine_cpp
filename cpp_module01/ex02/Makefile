CPP			= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98
RM			= rm -f
SRCS		= main.cpp
NAME		= brain
OBJS		= $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME): $(OBJS) 
	@$(CPP) $(CPPFLAGS) $< -o $@

%.o : %.cpp 
	@$(CPP) $(CPPFLAGS) -c $< -o $@

clean :
	@$(RM) $(OBJS)
fclean : clean
	@$(RM) $(NAME)
re : fclean all

.PHONY: all clean fclean re