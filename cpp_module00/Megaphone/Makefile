# Project Configuration
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror
INC = -I/mnt/c/Users/dylan/Shared/picine_cpp/libcpp
LDFLAGS =
LIB_DIR = /mnt/c/Users/dylan/Shared/picine_cpp/libcpp

# Source and Build Directories
SRC = megaphone.cpp
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
TARGET = $(BIN_DIR)/ex00

# Object Files
OBJ = $(SRC:.cpp=.o)
OBJ := $(OBJ:.c=.o)
OBJ := $(addprefix $(OBJ_DIR)/,$(OBJ))

# Google Test Configuration
GTEST_DIR = ../../vendor/gtest
GTEST_INC_CANDIDATES = $(GTEST_DIR)/include $(GTEST_DIR)/googletest/include $(GTEST_DIR)
GTEST_INC := $(shell for d in $(GTEST_INC_CANDIDATES); do [ -d "$$d" ] && { echo -I$$d; break; } done)
ifeq ($(strip $(GTEST_INC)),)
GTEST_INC = -I$(GTEST_DIR)/include
endif
GTEST_LIBS = -L$(GTEST_DIR)/build/lib -L$(GTEST_DIR)/lib -lgtest -lgtest_main -pthread

# Checker Script
CHECKER = /mnt/c/Users/dylan/Shared/picine_cpp/vendor/scripts/checker.py

# Test Configuration
TEST_SRCS := $(wildcard tests/*.cpp)
TEST_BINS := $(patsubst tests/%.cpp,$(BIN_DIR)/%,$(TEST_SRCS))
TEST_OBJS := $(patsubst tests/%.cpp,$(OBJ_DIR)/tests/%.o,$(TEST_SRCS))

# Auto-detect include directories if not set
ifeq ($(strip $(INC)),)
INC := -I$(LIB_DIR) -I$(LIB_DIR)/include -I$(LIB_DIR)/inc
endif

CPPFLAGS :=

# Colors and Formatting
RESET = \033[0m
BOLD = \033[1m
DIM = \033[2m
GREEN = \033[92m
CYAN = \033[96m
YELLOW = \033[93m
GRAY = \033[90m
RED = \033[91m

# Progress tracking
TOTAL_SRCS := $(words $(SRC))
COMPILED := 0

# Spinner frames
SPINNER = ⠋ ⠙ ⠹ ⠸ ⠼ ⠴ ⠦ ⠧ ⠇ ⠏

# Main targets
all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	@printf "  $(DIM)linking$(RESET)      $(CYAN)$(notdir $@)$(RESET)\n"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) -o $@ $(LDFLAGS) $(LIBS)
	@printf "\n  $(GREEN)●$(RESET) $(BOLD)Build complete$(RESET) $(DIM)→ $@$(RESET)\n\n"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@$(eval COMPILED:=$(shell echo $$(($(COMPILED)+1))))
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@$(eval COMPILED:=$(shell echo $$(($(COMPILED)+1))))
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) -x c++ -c $< -o $@

$(OBJ_DIR)/tests/%.o: tests/%.cpp | $(OBJ_DIR)/tests
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(GTEST_INC) -I. -c $< -o $@

# Build each test binary
$(BIN_DIR)/%: $(OBJ_DIR)/tests/%.o $(OBJ) | $(BIN_DIR)
	@printf "  $(DIM)linking$(RESET)      $(CYAN)%-30s$(RESET)\n" "$(notdir $@)"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(INC) $(OBJ) $< -o $@ $(GTEST_LIBS) $(LDFLAGS) $(LIBS)
	@printf "  $(GREEN)●$(RESET) $(BOLD)Test build complete$(RESET) $(DIM)→ $@$(RESET)\n"

# test: build all test binaries
test: gtest $(TEST_BINS)

# Google Test
gtest:
	@if [ ! -d "$(GTEST_DIR)/build" ]; then \
		printf "  $(DIM)creating$(RESET)     $(YELLOW)gtest build directory$(RESET)\n"; \
		mkdir -p "$(GTEST_DIR)/build"; \
	fi
	@if [ ! -f "$(GTEST_DIR)/lib/libgtest.a" ] || [ ! -f "$(GTEST_DIR)/lib/libgtest_main.a" ]; then \
		printf "  $(DIM)building$(RESET)     $(YELLOW)Google Test$(RESET)\n"; \
		cd "$(GTEST_DIR)/build" && cmake .. > /dev/null 2>&1 && $(MAKE) > /dev/null 2>&1; \
		mkdir -p "$(GTEST_DIR)/lib"; \
		cp "$(GTEST_DIR)/build/lib/libgtest.a" "$(GTEST_DIR)/lib/" 2>/dev/null || true; \
		cp "$(GTEST_DIR)/build/lib/libgtest_main.a" "$(GTEST_DIR)/lib/" 2>/dev/null || true; \
		printf "  $(GREEN)●$(RESET) Google Test ready\n\n"; \
	fi

# Clean targets
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		printf "  $(DIM)cleaning$(RESET)     $(YELLOW)object files$(RESET)\n"; \
		rm -rf $(OBJ_DIR); \
	fi

fclean: clean
	@if [ -d "$(BIN_DIR)" ]; then \
		printf "  $(DIM)cleaning$(RESET)     $(YELLOW)binaries$(RESET)\n"; \
		rm -rf $(BIN_DIR); \
	fi
	@if [ -d "$(BUILD_DIR)" ]; then \
		rmdir $(BUILD_DIR) 2>/dev/null || true; \
	fi
	@printf "  $(GREEN)●$(RESET) Clean complete\n\n"

re: fclean all

# Diagnostics
diag:
	@printf "\n$(BOLD)$(CYAN)Build Configuration$(RESET)\n"
	@printf "$(DIM)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n"
	@printf "  $(DIM)CXX$(RESET)          $(CYAN)$(CXX)$(RESET)\n"
	@printf "  $(DIM)CXXFLAGS$(RESET)     $(CYAN)$(CXXFLAGS)$(RESET)\n"
	@printf "  $(DIM)INC$(RESET)          $(CYAN)$(INC)$(RESET)\n"
	@printf "  $(DIM)LIB_DIR$(RESET)      $(CYAN)$(LIB_DIR)$(RESET)\n"
	@printf "  $(DIM)SRC$(RESET)          $(CYAN)$(SRC)$(RESET)\n"
	@printf "  $(DIM)OBJ$(RESET)          $(CYAN)$(OBJ)$(RESET)\n"
	@printf "  $(DIM)TARGET$(RESET)       $(CYAN)$(TARGET)$(RESET)\n"
	@printf "$(DIM)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n\n"

norminette:
	$(CHECKER) $(shell find . -path ./tests -prune -o -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' -o -name '*.h' -o -name '*.hpp' -o -name '*.hh' \) -print) -- -std=c++98 -I.

# Directory creation
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

$(OBJ_DIR)/tests:
	@mkdir -p $@

.PHONY: all clean fclean re diag norminette test gtest
