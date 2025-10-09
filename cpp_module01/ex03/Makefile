CPP			= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98
RM			= rm -f
RMDIR		= rm -rf
NAME		= violence
SRCS		= HumanA.cpp HumanB.cpp main.cpp Weapon.cpp
OBJ_DIR		= obj
# place object files inside $(OBJ_DIR)
OBJS		= $(addprefix $(OBJ_DIR)/,$(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPPFLAGS) $^ -o $@

# build rule: compile src/*.cpp -> obj/*.o
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

# ensure obj directory exists
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	$(RMDIR) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: clean fclean re all