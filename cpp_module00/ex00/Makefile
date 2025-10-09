NAME=megaphone
CXX = c++
CXXFLAGS = -Wall -Wall -Werror -std=c++98
CPPFLAGS ?=

SRCS= megaphone.cpp
OBJS= $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@ 

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re : fclean all

.PHONY: all clean fclean re
	

