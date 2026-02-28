# Project Configuration
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror
INC = 
LDFLAGS = 
LIB_DIR = 
LIBS = 

# Add conditional -L flag only when LIB_DIR is set
LIBDIR_FLAG := $(if $(strip ),-L,)

# Source files (auto-detected or provided)
SRC_LIST = Replacer.cpp main.cpp

# Build directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# CRITICAL FIX: Isolate the Python target injection so rules compile to the right folder
EXEC_BIN = $(BIN_DIR)/ex04

# Object files from SRC_LIST
OBJ := $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(SRC_LIST))))

# Exclude main.o from the object list so tests don't get double main() functions
OBJ_NO_MAIN := $(filter-out %/main.o %/Main.o,$(OBJ))

# Google Test Configuration
GTEST_DIR = ../../vendor/gtest
GTEST_INC_CANDIDATES = ../../vendor/gtest/include ../../vendor/gtest/googletest/include ../../vendor/gtest
GTEST_INC := $(shell for d in $(GTEST_INC_CANDIDATES); do [ -d "$$d" ] && { echo -I$$d; break; } done)
ifeq ($(strip $(GTEST_INC)),)
GTEST_INC = -I../../vendor/gtest/include
endif
GTEST_LIBS = -L../../vendor/gtest/build/lib -L../../vendor/gtest/lib -lgtest -lgtest_main -pthread

# Checker Script
CHECKER = /path/to/checker.py

# Test Configuration
TEST_SRCS := $(wildcard tests/*.cpp tests/*.c)
TEST_BINS := $(foreach f,tests/test.cpp,$(BIN_DIR)/$(basename $(notdir $(f))))

# libcpp control flag
LCPP ?= 0

# Auto-detect include directories
ifeq ($(strip ),)
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
		c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(OBJ) -o $@  $$libfile; \
	else \
		c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(OBJ) -o $@ $(LIBDIR_FLAG)  ; \
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
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  -x c++ -c $< -o $@

# ── Test targets ──────────────────────────────────────────────────────────────
# Compile test sources
$(OBJ_DIR)/tests/%.o: tests/%.cpp | $(OBJ_DIR)/tests
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling test$(RESET) $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(GTEST_INC) -I. -c $< -o $@

# Link each test binary: Uses OBJ_NO_MAIN to avoid conflicts!
$(BIN_DIR)/%: $(OBJ_DIR)/tests/%.o $(OBJ_NO_MAIN) | $(BIN_DIR)
	@printf "  $(DIM)linking test$(RESET)   $(CYAN)%-30s$(RESET)\n" "$(notdir $@)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(OBJ_NO_MAIN) $< -o $@ $(GTEST_LIBS)  
	@printf "  $(GREEN)●$(RESET) $(BOLD)Test build complete$(RESET) $(DIM)→ $@$(RESET)\n"

test: gtest $(BIN_DIR)/test
	@printf "\n  $(GREEN)●$(RESET) Built $(words $(BIN_DIR)/test) test(s)\n"
	@for test_bin in $(BIN_DIR)/test; do \
		echo "  $(CYAN)Executing $$test_bin...$(RESET)"; \
		./$$test_bin || exit 1; \
	done

# Google Test build helper
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

.PHONY: all run clean fclean re test gtest
