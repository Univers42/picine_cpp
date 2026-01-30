NAME    = replace
CXX     = c++
CXXFLAGS= -Wall -Wextra -Werror -std=c++98
SRCS    = main.cpp scanner.cpp
OBJS    = $(SRCS:.cpp=.o)
DEPS    = Scanner.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME) input.txt "pattern" "replacement"

.PHONY: all clean fclean re run
