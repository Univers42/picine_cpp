CPP = c++
CPPFLAGS= -Wall -Wextra -Wall -std=c++98
NAME=zombie_horde

SRCS = main.cpp Zombie.cpp zombieHorde.cpp
OBJS = $(SRCS:.cpp=.o)
all:$(NAME)

$(NAME):$(OBJS)
	$(CPP) $(CPPFLAGS) $^ -o $@
	
%.o : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean:clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re