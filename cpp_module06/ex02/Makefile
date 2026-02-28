CXX=c++
CXXFLAGS=-Wall -Wextra -Werror
INC=-I/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LDFLAGS=
LIB_DIR=/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LIBS=-lcpp
SRC=Function.cpp main.cpp 

# place object files in a dedicated directory
OBJDIR ?= obj
OBJ := $(addprefix $(OBJDIR)/,$(addsuffix .o,$(basename $(SRC))))
OBJ := $(OBJ)

TARGET=IdentityRealType

BUILD_LIBCPP ?= 0

# If INC wasn't provided by env, add common candidate include dirs inside $(LIB_DIR)
ifeq ($(strip $(INC)),)
ifeq ($(BUILD_LIBCPP),0)
INC :=
else
INC := -I$(LIB_DIR) -I$(LIB_DIR)/include -I$(LIB_DIR)/inc
endif
endif

# If libcpp provides a single top-level header, pre-include it so declarations are available
ifeq ($(BUILD_LIBCPP),0)
CPPFLAGS :=
else
ifeq ($(wildcard $(LIB_DIR)/libcpp.h),)
CPPFLAGS :=
else
CPPFLAGS := -include $(LIB_DIR)/libcpp.h
endif
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	@if [ "$(BUILD_LIBCPP)" != "0" ]; then $(MAKE) -C $(LIB_DIR); fi; \
	@libfile=""; \
	if [ -f "$(LIB_DIR)/libcpp.a" ]; then \
		libfile="$(LIB_DIR)/libcpp.a"; \
	elif [ -f "$(LIB_DIR)/libcpp.so" ]; then \
		libfile="$(LIB_DIR)/libcpp.so"; \
	fi; \
	if [ "$(BUILD_LIBCPP)" != "0" ]; then \
		libobjs="$$(ls $(LIB_DIR)/*.o 2>/dev/null || true)"; \
	else \
		libobjs=""; \
	fi; \
	if [ -n "$$libobjs" ] && [ "$(BUILD_LIBCPP)" != "0" ]; then \
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) $$libobjs -o $(TARGET) $(LDFLAGS) $$libfile; \
	elif [ -n "$$libfile" ]; then \
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) -o $(TARGET) $(LDFLAGS) $$libfile; \
	else \
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) -o $(TARGET) -L$(LIB_DIR) $(LDFLAGS) $(LIBS); \
	fi

# object rules: ensure obj dir exists then compile
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) -x c++ -c $< -o $@

clean:
	rm -f $(OBJ)
	-rmdir --ignore-fail-on-non-empty $(OBJDIR) 2>/dev/null || rm -rf $(OBJDIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re