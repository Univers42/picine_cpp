CXX=c++
CXXFLAGS=-Wall -Wextra -Werror
INC=-I/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LDFLAGS=
LIB_DIR=/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LIBS=-lcpp
SRC=
OBJ=$(SRC:.cpp=.o)
TARGET=.

# default directories to generate Makefiles for (kept for reference)
GEN_DIRS := . cpp_module00/ex01

# capture positional args passed after "make genmake"
ARG_DIRS := $(filter-out genmake,$(MAKECMDGOALS))
DIRS := $(ARG_DIRS)

# Only trigger an error if user invoked "make genmake" without args
ifneq (,$(filter genmake,$(MAKECMDGOALS)))
  ifeq ($(ARG_DIRS),)
    $(error genmake requires at least one directory argument. Usage: make genmake dir1 dir2 ...)
  endif
endif

# create no-op targets for positional args so make doesn't try to build them
ifeq ($(ARG_DIRS),)
else
$(foreach d,$(ARG_DIRS),$(eval $(d): ; @:))
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(LIB_DIR)
	$(CXX) $(CXXFLAGS) $(INC) $(OBJ) -o $(TARGET) -L$(LIB_DIR) $(LDFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C libcpp clean || true
	$(MAKE) -C cpp_module00/ex01 clean || true

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re genmake

genmake:
	@for d in $(DIRS); do \
		printf "[GEN] generating Makefile for %s\n" $$d; \
		./autotools/env.sh autotools "$$d"; \
	done