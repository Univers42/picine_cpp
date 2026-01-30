CPP		= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98
NAME		= zombie

SRCS		= main.cpp Zombie.cpp zombieHorde.cpp
OBJ_DIR		= build/obj
BIN_DIR		= build/bin
OBJS		= $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(BIN_DIR)/$(NAME)

$(BIN_DIR)/$(NAME): $(OBJS) | $(BIN_DIR)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(BIN_DIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
