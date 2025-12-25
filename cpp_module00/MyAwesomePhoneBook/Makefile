CXX=c++
CXXFLAGS=-Wall -Wextra -Werror
# compute include path from LIB_DIR (prefers include/, then inc/, then lib dir)
INC := $(shell if [ -d "$(LIB_DIR)/include" ]; then echo -I$(LIB_DIR)/include; elif [ -d "$(LIB_DIR)/inc" ]; then echo -I$(LIB_DIR)/inc; else echo -I$(LIB_DIR); fi)
LDFLAGS=
LIB_DIR=/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LIBS=-lcpp
SRC=Contact.cpp main.cpp PhoneBook.cpp 
OBJ=$(SRC:.cpp=.o)
TARGET=ex01

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(LIB_DIR)
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) -o $(TARGET) -L$(LIB_DIR) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re