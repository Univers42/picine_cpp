CXX=c++
CXXFLAGS=-Wall -Wextra -Werror
LDFLAGS=
SRC=main.cpp ClapTrap.cpp 
OBJ=$(SRC:.cpp=.o)
TARGET=claptrap

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re