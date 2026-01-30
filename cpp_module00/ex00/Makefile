# Remote URLs for vendor dependencies
GTEST_URL := https://github.com/google/googletest.git
LIBCPP_URL := git@github.com:Univers42/libcpp.git
SCRIPTS_URL := git@github.com:Univers42/scripts.git

# Variables
CXX := c++
CXXFLAGS := -Wall -Wextra -Werror
# Only include root .cpp files in SRCS (not tests/*.cpp)
SRCS := megaphone.cpp
NAME := megaphone
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BIN_DIR)/$(NAME)
INCS_DIR := -I.

# Vendor directories (relative to module root)
VENDOR_DIR := ../vendor
GTEST_DIR := $(VENDOR_DIR)/gtest
LIBCPP_DIR := $(VENDOR_DIR)/libcpp
SCRIPTS_DIR := $(VENDOR_DIR)/scripts
GTEST_LIB := $(GTEST_DIR)/lib/libgtest.a
GTEST_MAIN_LIB := $(GTEST_DIR)/lib/libgtest_main.a
GTEST_INC := -I$(GTEST_DIR)/include -I$(GTEST_DIR)/googletest/include

# Test sources and binaries
TEST_SRCS := $(wildcard tests/*.cpp)
TEST_BINS := $(patsubst tests/%.cpp,$(BIN_DIR)/%,$(TEST_SRCS))
TEST_OBJS := $(patsubst tests/%.cpp,$(OBJ_DIR)/tests/%.o,$(TEST_SRCS))

RM := rm -rf
# Object files
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
# Exclude main.o for test linking
OBJS_NO_MAIN := $(filter-out $(OBJ_DIR)/main.o,$(OBJS))

CXXSTD98 := -std=c++98
CXXSTD17 := -std=c++17

# Colors and Formatting
RESET = \033[0m
BOLD = \033[1m
DIM = \033[2m
GREEN = \033[92m
CYAN = \033[96m
YELLOW = \033[93m
GRAY = \033[90m
RED = \033[91m

# Default target: build the megaphone binary
all: main

objs_only: $(OBJS)
	@printf "  $(GREEN)●$(RESET) $(BOLD)All object files built (no binaries).$(RESET)\n"

# Main program build (explicit, always c++98)
.PHONY: main
main: $(OBJS) | $(BIN_DIR)
	@printf "  $(DIM)linking$(RESET)      $(CYAN)$(NAME)$(RESET)\n"
	@$(CXX) $(CXXFLAGS) -std=c++98 $(OBJS) -o $(BIN_DIR)/$(NAME)
	@printf "\n  $(GREEN)●$(RESET) $(BOLD)Main program built$(RESET) $(DIM)→ $(BIN_DIR)/$(NAME)$(RESET)\n\n"

# Implicit rule for object files
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(CXXSTD98) $(INCS_DIR) -c $< -o $@

# Directory creation
$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

# Test build: ensure vendor, gtest, and libcpp are present
.PHONY: test

test: vendor $(GTEST_LIB) $(GTEST_MAIN_LIB) $(TEST_BINS)

# Build all test binaries
$(BIN_DIR)/%: $(OBJ_DIR)/tests/%.o $(OBJS_NO_MAIN) $(GTEST_LIB) $(GTEST_MAIN_LIB) | $(BIN_DIR)
	@printf "  $(DIM)linking test$(RESET)   $(CYAN)%-30s$(RESET)\n" "$(notdir $@)"
	@$(CXX) $(CXXFLAGS) $(CXXSTD17) $^ -o $@ $(GTEST_INC) -L$(GTEST_DIR)/lib -lgtest -lgtest_main -pthread 2>&1
	@printf "  $(GREEN)●$(RESET) $(BOLD)Test build complete$(RESET) $(DIM)→ $@$(RESET)\n"

# Implicit rule for test object files
$(OBJ_DIR)/tests/%.o: tests/%.cpp | $(OBJ_DIR)/tests
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling test$(RESET) $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@$(CXX) $(CXXFLAGS) $(CXXSTD17) $(INCS_DIR) $(GTEST_INC) -c $< -o $@

$(OBJ_DIR)/tests:
	@mkdir -p $@

# Vendor setup
.PHONY: vendor
vendor: $(VENDOR_DIR) $(GTEST_DIR) $(LIBCPP_DIR) $(SCRIPTS_DIR)

$(VENDOR_DIR):
	@mkdir -p $@

$(GTEST_DIR): | $(VENDOR_DIR)
	@if [ ! -d "$@" ]; then git clone $(GTEST_URL) $@; fi
	@mkdir -p $@/build

$(LIBCPP_DIR): | $(VENDOR_DIR)
	@if [ ! -d "$@" ]; then git clone $(LIBCPP_URL) $@; fi

$(SCRIPTS_DIR): | $(VENDOR_DIR)
	@if [ ! -d "$@" ]; then git clone $(SCRIPTS_URL) $@; fi

# Build Google Test static libraries if missing
$(GTEST_LIB) $(GTEST_MAIN_LIB): | $(GTEST_DIR)
	@if [ ! -f "$(GTEST_LIB)" ] || [ ! -f "$(GTEST_MAIN_LIB)" ]; then \
		cd $(GTEST_DIR)/build && cmake .. && $(MAKE) && \
		mkdir -p ../lib && \
		cp lib/libgtest.a ../lib/ && \
		cp lib/libgtest_main.a ../lib/; \
	fi

# Python virtual environment for diagnostics
VENV_DIR := .venv
VENV_PY := $(VENV_DIR)/bin/python3
VENV_PIP := $(VENV_DIR)/bin/pip3
VENV_ACT := . $(VENV_DIR)/bin/activate

.PHONY: virt_env shut_virt_env diagnostic

virt_env:
	@if [ ! -d $(VENV_DIR) ]; then \
		printf "  $(DIM)creating$(RESET)     $(YELLOW)python venv$(RESET)\n"; \
		python3 -m venv $(VENV_DIR); \
	fi
	@printf "  $(DIM)installing$(RESET)   $(YELLOW)python tools$(RESET)\n"
	@$(VENV_PIP) install --upgrade pip > /dev/null 2>&1
	@$(VENV_PIP) install clang-format clang-tidy cppcheck cpplint > /dev/null 2>&1
	@printf "  $(GREEN)●$(RESET) $(BOLD)Python venv ready$(RESET)\n"

shut_virt_env:
	@if [ -d $(VENV_DIR) ]; then \
		printf "  $(DIM)removing$(RESET)     $(YELLOW)python venv$(RESET)\n"; \
		rm -rf $(VENV_DIR); \
		printf "  $(GREEN)●$(RESET) $(BOLD)Python venv removed$(RESET)\n"; \
	fi

diagnostic: virt_env vendor megaphone.cpp
	@bash -c '. $(VENV_DIR)/bin/activate && python3 ../vendor/scripts/checker.py $(SRCS) -- -std=c++98 -I.'

# Clean targets
.PHONY: clean fclean re
clean:
	@if [ -d "$(OBJ_DIR)" ]; then \
		printf "  $(DIM)cleaning$(RESET)     $(YELLOW)object files$(RESET)\n"; \
		$(RM) -r $(OBJ_DIR); \
	fi

fclean:
	@if [ -d "$(BUILD_DIR)" ]; then \
		printf "  $(DIM)cleaning$(RESET)     $(YELLOW)build directory$(RESET)\n"; \
		$(RM) -r $(BUILD_DIR); \
	fi
	@if [ -d $(VENV_DIR) ]; then \
		printf "  $(DIM)removing$(RESET)     $(YELLOW)python venv$(RESET)\n"; \
		$(RM) -rf $(VENV_DIR); \
	fi
	@if [ -d $(VENDOR_DIR) ]; then \
		printf "  $(DIM)removing$(RESET)     $(YELLOW)vendor directory$(RESET)\n"; \
		$(RM) -rf $(VENDOR_DIR); \
	fi
	@printf "  $(GREEN)●$(RESET) $(BOLD)Full clean complete$(RESET)\n"

re: fclean all

# Help
.PHONY: help
help:
	@echo "Usage: make [all|test|clean|fclean|re|help]"
	@echo "  all   - build main program only (no vendor deps)"
	@echo "  test  - build and run tests (with vendor deps)"
	@echo "  clean - remove object files"
	@echo "  fclean- remove all build files"
	@echo "  re    - clean and rebuild main"

.PHONY: auto_src

auto_src:
	@srcs=`find . -maxdepth 2 -name "*.cpp" | sed 's|\./||' | tr '\n' ' ' | sed 's/ *$$//'`; \
	sed -i.bak "/^SRCS :=.*/c\SRCS := $$srcs" Makefile; \
	printf "  $(GREEN)●$(RESET) $(BOLD)SRCS updated in Makefile:$(RESET) SRCS := $$srcs\n"
