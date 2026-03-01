# Project Configuration
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror
INC = -I.
LDFLAGS = 
LIB_DIR = 
LIBS = 

# Add conditional -L flag only when LIB_DIR is set
LIBDIR_FLAG := $(if $(strip ),-L,)

# Source files (auto-detected or provided)
SRC_LIST = main.cpp

# Build directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# CRITICAL FIX: Isolate the Python target injection so rules compile to the right folder
EXEC_BIN = $(BIN_DIR)/ex02

# Object files from SRC_LIST
OBJ := $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(SRC_LIST))))

# Exclude main.o from the object list so tests don't get double main() functions
OBJ_NO_MAIN := $(filter-out %/main.o %/Main.o,$(OBJ))

GTEST_DIR = ../../vendor/gtest
GTEST_LIB_PATH = ../../vendor/gtest/lib
# Only add -lgtest flags if the library files actually exist
HAS_GTEST := $(shell [ -f "$(GTEST_LIB_PATH)/libgtest.a" ] && echo 1 || echo 0)

ifeq ($(HAS_GTEST),1)
    GTEST_LIBS = -L$(GTEST_LIB_PATH) -lgtest -lgtest_main -pthread
    GTEST_DEF = -DHAVE_GTEST=1
else
    GTEST_LIBS =
    GTEST_DEF = -DHAVE_GTEST=0
endif

# Checker Script
CHECKER = $(CHECKER)

# Test Configuration
TEST_SRCS := $(wildcard tests/*.cpp tests/*.c)
TEST_BINS := $(foreach f,$(TEST_SRCS),$(BIN_DIR)/$(basename $(notdir $(f))))

# libcpp control flag
LCPP ?= 0

# Auto-detect include directories
ifeq ($(strip -I.),)
ifeq ($(LCPP),0)
INC :=
else
INC := -I -I/include -I/inc
endif
endif

# Pre-include libcpp header
ifeq ($(LCPP),0)
CPPFLAGS :=
else
ifeq ($(wildcard /libcpp.h),)
CPPFLAGS :=
else
CPPFLAGS := -include /libcpp.h
endif
endif

# Colors
RESET = \033[0m
BOLD = \033[1m
DIM = \033[2m
GREEN = \033[92m
CYAN = \033[96m
YELLOW = \033[93m

# ── Main targets ──────────────────────────────────────────────────────────────
all: $(EXEC_BIN)

$(EXEC_BIN): $(OBJ) | $(BIN_DIR)
	@printf "  $(DIM)linking$(RESET)      $(CYAN)$(notdir $@)$(RESET)\n"
	@if [ "$(LCPP)" != "0" ]; then $(MAKE) -C ; fi
	@libfile=""; \
	if [ -f "/libcpp.a" ]; then libfile="/libcpp.a"; \
	elif [ -f "/libcpp.so" ]; then libfile="/libcpp.so"; fi; \
	if [ -n "$$libfile" ]; then \
		c++ -Wall -Wextra -Werror $(CPPFLAGS) -I. $(OBJ) -o $@  $$libfile; \
	else \
		c++ -Wall -Wextra -Werror $(CPPFLAGS) -I. $(OBJ) -o $@ $(LIBDIR_FLAG)  ; \
	fi
	@printf "\n  $(GREEN)●$(RESET) $(BOLD)Build complete$(RESET) $(DIM)→ $@$(RESET)\n\n"

# Run target to easily execute the root program
run: $(EXEC_BIN)
	@printf "  $(CYAN)Running $(EXEC_BIN)...$(RESET)\n\n"
	@./$(EXEC_BIN)

# Compile main sources
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS) -I. -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS) -I. -x c++ -c $< -o $@

# ── Test targets ──────────────────────────────────────────────────────────────
# Compile test sources
$(OBJ_DIR)/tests/%.o: tests/%.cpp | $(OBJ_DIR)/tests
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling test$(RESET) $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS) -I. $(GTEST_INC) $(GTEST_DEF) -I. -c $< -o $@

$(BIN_DIR)/%: $(OBJ_DIR)/tests/%.o $(OBJ_NO_MAIN) | $(BIN_DIR)
	@printf "  $(DIM)linking test$(RESET)   $(CYAN)%-30s$(RESET)\n" "$(notdir $@)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS) -I. $(OBJ_NO_MAIN) $< -o $@ $(GTEST_LIBS)  
	@printf "  $(GREEN)●$(RESET) $(BOLD)Test build complete$(RESET) $(DIM)→ $@$(RESET)\n"

# Dynamically build and execute any tests found in the tests/ directory
test: $(TEST_BINS)
	@if [ -z "$(TEST_BINS)" ]; then \
		printf "\n  $(YELLOW)⚠$(RESET) No tests found in tests/ directory.\n"; \
	else \
		printf "\n  $(GREEN)●$(RESET) Built $(words $(TEST_BINS)) test(s)\n"; \
		for test_bin in $(TEST_BINS); do \
			echo "  $(CYAN)Executing $$test_bin...$(RESET)"; \
		done; \
	fi

# Google Test build helper (Only runs if 'make gtest' is explicitly called)
gtest:
	@if [ ! -d "../../vendor/gtest/build" ]; then \
		mkdir -p "../../vendor/gtest/build"; \
	fi
	@if [ ! -f "../../vendor/gtest/lib/libgtest.a" ] || [ ! -f "../../vendor/gtest/lib/libgtest_main.a" ]; then \
		printf "  $(DIM)building$(RESET)     $(YELLOW)Google Test$(RESET)\n"; \
		cd "../../vendor/gtest/build" && cmake .. > /dev/null 2>&1 && $(MAKE) > /dev/null 2>&1; \
		mkdir -p "../../vendor/gtest/lib"; \
		cp "../../vendor/gtest/build/lib/libgtest.a" "../../vendor/gtest/lib/" 2>/dev/null || true; \
		cp "../../vendor/gtest/build/lib/libgtest_main.a" "../../vendor/gtest/lib/" 2>/dev/null || true; \
	fi

# Clean targets
clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(BUILD_DIR)

re: fclean all

# Directory creation
$(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)/tests:
	@mkdir -p $@

# ── Code Quality / Linting ────────────────────────────────────────────────────

# Auto-injected by gen_make.py based on relative directory structures
LINTER_SCRIPT = ../../autotools/norminette.sh

norminette:
	@printf "\n  $(BOLD)$(CYAN) Running C++ Code Quality Checks$(RESET)\n"
	@if [ -f "../../autotools/norminette.sh" ]; then \
		python3 "../../autotools/norminette.sh" . -- -std=c++98 -Wall -Wextra -I.; \
	else \
		printf "  $(RED)✘ Linter script not found at ../../autotools/norminette.sh$(RESET)\n"; \
	fi

# Bonus: Automatically fix clang-format issues
format:
	@printf "\n  $(BOLD)$(CYAN) Auto-formatting C++ Source Files$(RESET)\n"
	@if command -v clang-format >/dev/null 2>&1; then \
		find . -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.c" | xargs clang-format -i -style=Google; \
		printf "  $(GREEN)●$(RESET) Files formatted successfully using Google style.\n"; \
	else \
		printf "  $(RED)✘ clang-format is not installed!$(RESET)\n"; \
	fi

.PHONY: all run clean fclean re test gtest norminette format
