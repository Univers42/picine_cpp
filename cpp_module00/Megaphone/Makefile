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
SRC_LIST = megaphone.cpp

# Build directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
TARGET = $(BIN_DIR)/Megaphone

# Object files
OBJ := $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(basename $(SRC_LIST))))

# Google Test Configuration
GTEST_DIR = ../../vendor/gtest
GTEST_INC_CANDIDATES = ../../vendor/gtest/include ../../vendor/gtest/googletest/include ../../vendor/gtest
GTEST_INC := $(shell for d in $(GTEST_INC_CANDIDATES); do [ -d "$$d" ] && { echo -I$$d; break; } done)
ifeq ($(strip $(GTEST_INC)),)
GTEST_INC = -I../../vendor/gtest/include
endif
GTEST_LIBS = -L../../vendor/gtest/build/lib -L../../vendor/gtest/lib -lgtest -lgtest_main -pthread

# Checker Script
CHECKER = ../../vendor/scripts/checker.py

# Test Configuration
TEST_SRCS := $(wildcard tests/*.cpp)
TEST_BINS := $(patsubst tests/%.cpp,$(BIN_DIR)/%,tests/fuzzy_test.cpp tests/test_megaphone.cpp)
TEST_OBJS := $(patsubst tests/%.cpp,$(OBJ_DIR)/tests/%.o,tests/fuzzy_test.cpp tests/test_megaphone.cpp)

# libcpp control flag (set LCPP=1 to enable)
LCPP ?= 0

# Auto-detect include directories if not set
ifeq ($(strip ),)
ifeq ($(LCPP),0)
INC :=
else
INC := -I -I/include -I/inc
endif
endif

# Pre-include libcpp header if available and enabled
ifeq ($(LCPP),0)
CPPFLAGS :=
else
ifeq ($(wildcard /libcpp.h),)
CPPFLAGS :=
else
CPPFLAGS := -include /libcpp.h
endif
endif

# Colors and Formatting
RESET = \033[0m
BOLD = \033[1m
DIM = \033[2m
GREEN = \033[92m
CYAN = \033[96m
YELLOW = \033[93m
GRAY = \033[90m
RED = \033[91m

# Main targets
all: Megaphone

Megaphone: $(OBJ) | $(BIN_DIR)
	@printf "  $(DIM)linking$(RESET)      $(CYAN)$(notdir $@)$(RESET)\n"
	@if [ "$(LCPP)" != "0" ]; then $(MAKE) -C ; fi
	@libfile=""; \
	if [ -f "/libcpp.a" ]; then \
		libfile="/libcpp.a"; \
	elif [ -f "/libcpp.so" ]; then \
		libfile="/libcpp.so"; \
	fi; \
	if [ "$(LCPP)" != "0" ]; then \
		libobjs="$$(ls /*.o 2>/dev/null || true)"; \
	else \
		libobjs=""; \
	fi; \
	if [ -n "$$libobjs" ] && [ "$(LCPP)" != "0" ]; then \
		c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(OBJ) $$libobjs -o $@  $$libfile; \
	elif [ -n "$$libfile" ]; then \
		c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(OBJ) -o $@  $$libfile; \
	else \
		c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(OBJ) -o $@ $(LIBDIR_FLAG)  ; \
	fi
	@printf "\n  $(GREEN)●$(RESET) $(BOLD)Build complete$(RESET) $(DIM)→ $@$(RESET)\n\n"

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  -x c++ -c $< -o $@

$(OBJ_DIR)/tests/%.o: tests/%.cpp | $(OBJ_DIR)/tests
	@printf "  $(DIM)compiling$(RESET)    $(CYAN)%-30s$(RESET)\n" "$(notdir $<)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(GTEST_INC) -I. -c $< -o $@

$(BIN_DIR)/%: $(OBJ_DIR)/tests/%.o $(OBJ) | $(BIN_DIR)
	@printf "  $(DIM)linking$(RESET)      $(CYAN)%-30s$(RESET)\n" "$(notdir $@)"
	@c++ -Wall -Wextra -Werror $(CPPFLAGS)  $(OBJ) $< -o $@ $(GTEST_LIBS)  
	@printf "  $(GREEN)●$(RESET) $(BOLD)Test build complete$(RESET) $(DIM)→ $@$(RESET)\n"

test: gtest $(BIN_DIR)/fuzzy_test $(BIN_DIR)/test_megaphone

# Google Test
gtest:
	@if [ ! -d "../../vendor/gtest/build" ]; then \
		printf "  $(DIM)creating$(RESET)     $(YELLOW)gtest build directory$(RESET)\n"; \
		mkdir -p "../../vendor/gtest/build"; \
	fi
	@if [ ! -f "../../vendor/gtest/lib/libgtest.a" ] || [ ! -f "../../vendor/gtest/lib/libgtest_main.a" ]; then \
		printf "  $(DIM)building$(RESET)     $(YELLOW)Google Test$(RESET)\n"; \
		cd "../../vendor/gtest/build" && cmake .. > /dev/null 2>&1 && $(MAKE) > /dev/null 2>&1; \
		mkdir -p "../../vendor/gtest/lib"; \
		cp "../../vendor/gtest/build/lib/libgtest.a" "../../vendor/gtest/lib/" 2>/dev/null || true; \
		cp "../../vendor/gtest/build/lib/libgtest_main.a" "../../vendor/gtest/lib/" 2>/dev/null || true; \
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
	@printf "  $(DIM)CXX$(RESET)          $(CYAN)c++$(RESET)\n"
	@printf "  $(DIM)CXXFLAGS$(RESET)     $(CYAN)-Wall -Wextra -Werror$(RESET)\n"
	@printf "  $(DIM)INC$(RESET)          $(CYAN)$(RESET)\n"
	@printf "  $(DIM)LIB_DIR$(RESET)      $(CYAN)$(RESET)\n"
	@printf "  $(DIM)LCPP$(RESET)         $(CYAN)$(LCPP)$(RESET) $(DIM)(libcpp enabled: $(if $(filter-out 0,$(LCPP)),yes,no))$(RESET)\n"
	@printf "  $(DIM)SRC$(RESET)          $(CYAN)megaphone.cpp$(RESET)\n"
	@printf "  $(DIM)OBJ$(RESET)          $(CYAN)$(OBJ)$(RESET)\n"
	@printf "  $(DIM)TARGET$(RESET)       $(CYAN)Megaphone$(RESET)\n"
	@printf "  $(DIM)TEST_SRCS$(RESET)    $(CYAN)tests/fuzzy_test.cpp tests/test_megaphone.cpp$(RESET)\n"
	@printf "  $(DIM)TEST_BINS$(RESET)    $(CYAN)$(BIN_DIR)/fuzzy_test $(BIN_DIR)/test_megaphone$(RESET)\n"
	@printf "$(DIM)━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━$(RESET)\n\n"

norminette:
	@../../vendor/scripts/checker.py $(shell find . -path ./tests -prune -o -type f \( -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' -o -name '*.h' -o -name '*.hpp' -o -name '*.hh' \) -print) -- -std=c++98 -I.

# Directory creation
$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

$(OBJ_DIR)/tests:
	@mkdir -p $@

.PHONY: all clean fclean re diag norminette test gtest
