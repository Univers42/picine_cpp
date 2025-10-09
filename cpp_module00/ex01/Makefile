CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
NAME = phoneBook

SRCS = main.c Contact.cpp PhoneBook.cpp
OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# compile .c as C++ source
%.o: %.c
	$(CXX) $(CXXFLAGS) -x c++ -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re