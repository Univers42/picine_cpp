CPP 		= 	c++
CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98
NAME		=	zombie

SRCS		=	main.cpp newZombie.cpp randomChump.cpp Zombie.cpp scope_test.cpp
OBJS		=	$(SRCS:%.cpp=%.o)

RM			= rm -f

all: $(NAME)

$(NAME):$(OBJS)
	@$(CPP) $(OBJS) -o $(NAME)

%.o : %.cpp
	@$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
