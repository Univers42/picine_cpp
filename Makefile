CXX=c++
CXXFLAGS=-Wall -Wextra -Werror
INC=-I/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LDFLAGS=
LIB_DIR=/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LIBS=-lcpp
SRC=
OBJ=$(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)
TARGET=picine_cpp

# If INC wasn't provided by env, add common candidate include dirs inside $(LIB_DIR)
ifeq ($(strip $(INC)),)
INC := -I$(LIB_DIR) -I$(LIB_DIR)/include -I$(LIB_DIR)/inc
endif

# If libcpp provides a single top-level header, pre-include it so declarations are available
ifeq ($(wildcard $(LIB_DIR)/libcpp.h),)
CPPFLAGS :=
else
CPPFLAGS := -include $(LIB_DIR)/libcpp.h
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(LIB_DIR)
	@# prefer linking lib object files if present, else prefer direct lib file, else fallback to -L/-l
	@libobjs="$$(ls $(LIB_DIR)/*.o 2>/dev/null || true)"; \
	libfile=""; \
	if [ -f "$(LIB_DIR)/libcpp.a" ]; then \
		libfile="$(LIB_DIR)/libcpp.a"; \
	elif [ -f "$(LIB_DIR)/libcpp.so" ]; then \
		libfile="$(LIB_DIR)/libcpp.so"; \
	fi; \
	if [ -n "$$libobjs" ]; then \
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) $$libobjs -o $(TARGET) $(LDFLAGS) $$libfile; \
	elif [ -n "$$libfile" ]; then \
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) -o $(TARGET) $(LDFLAGS) $$libfile; \
	else \
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) -o $(TARGET) -L$(LIB_DIR) $(LDFLAGS) $(LIBS); \
	fi

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@

# compile .c as C++ (project uses C sources compiled with C++ toolchain)
%.o: %.c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) -x c++ -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

genmake:
	@if [ -z "$(path)" ]; then \
		echo "Usage: make genmake path=your/target/dir"; \
		exit 1; \
	fi; \
	sh "$(CURDIR)/autotools/env.sh" autotools "$(path)"

.PHONY: all clean fclean re genmake