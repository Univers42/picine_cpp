# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/28 17:03:51 by marvin            #+#    #+#              #
#    Updated: 2026/01/28 18:12:49 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX=c++
CXXFLAGS=-Wall -Wextra -Werror
INC=-I/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LDFLAGS=
LIB_DIR=/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LIBS=-lcpp
SRC=megaphone.cpp
OBJ=$(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)
TARGET=ex00

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

diag:
	@echo "=== Variables ==="; \
	printf "CXX=%s\nCXXFLAGS=%s\nINC=%s\nLIB_DIR=%s\nLIBS=%s\nSRC=%s\nOBJ=%s\nTARGET=%s\n" "$(CXX)" "$(CXXFLAGS)" "$(INC)" "$(LIB_DIR)" "$(LIBS)" "$(SRC)" "$(OBJ)" "$(TARGET)"; \
	echo; echo "=== libcpp directory listing ==="; ls -la "$(LIB_DIR)" || true; \
	echo; echo "=== Generated Makefile (this dir) ==="; sed -n '1,200p' Makefile 2>/dev/null || echo "(no Makefile)"; \
	echo; echo "=== Dry-run build ==="; $(MAKE) -n

norminette:
	./checker.py -- -std=c++98 -I. $(wildcard, *.c)

.PHONY: all clean fclean re diag
