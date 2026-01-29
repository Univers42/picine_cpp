NAME_STATIC = libcpp.a
NAME_SHARED = libcpp.so
SRCS := $(wildcard *.cpp)
OBJDIR := obj
OBJS := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -fPIC

# Colors
BOLD := \033[1m
BLUE := \033[1;34m
YELLOW := \033[1;33m
RED := \033[1;31m
RESET := \033[0m

# Improved, single-line logging (use as: $(call LOG_INFO, message))
define LOG_INFO
	@printf "%b\n" "$(BOLD)$(BLUE)[INFO]$(RESET) $(strip $(1))"
endef

define LOG_WARN
	@printf "%b\n" "$(BOLD)$(YELLOW)[WARN]$(RESET) $(strip $(1))"
endef

define LOG_ERROR
	@printf "%b\n" "$(BOLD)$(RED)[ERROR]$(RESET) $(strip $(1))"
endef

# Centralized commands
COMPILE = $(CXX) $(CXXFLAGS) -c $< -o $@

# Default target
all: $(NAME_STATIC) $(NAME_SHARED)

# Static library
$(NAME_STATIC): $(OBJS)
	$(call LOG_INFO, Creating static library: $@)
	@ar rcs $@ $^

# Shared library
$(NAME_SHARED): $(OBJS)
	$(call LOG_INFO, Creating shared library: $@)
	@$(CXX) -shared -o $@ $^

# Object file rule -> put .o into $(OBJDIR)
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	$(call LOG_INFO, Compiling $<)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files
clean:
	$(call LOG_WARN, Cleaning object files)
	@rm -rf $(OBJDIR)

# Clean all generated files
fclean: clean
	$(call LOG_WARN, Removing libraries)
	@rm -f $(NAME_STATIC) $(NAME_SHARED)

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
