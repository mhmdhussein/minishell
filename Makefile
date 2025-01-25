# Program name
NAME = minishell

# Colors
GREEN = \033[32m
RED = \033[31m
MAGENTA = \033[35m
YELLOW = \033[33m
CYAN = \033[36m
RESET = \033[0m

# Compile Flags
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

# Libft
LIBFT = includes/libft/libft.a
LIBFT_DIR = includes/libft

# Source files
SRC_DIR = src/
SRC =	src/main.c \
		src/builtins/ft_cd.c \
		src/builtins/ft_echo.c \
		src/builtins/ft_env.c \
		src/builtins/ft_exit.c \
		src/builtins/ft_export.c \
		src/builtins/ft_pwd.c \
		src/builtins/ft_unset.c \
		src/exec/builtin.c \
		src/helpers/split/ft_split.c \
		src/helpers/split/processing_quotes.c \
		src/helpers/split/string_split_utils.c \
		src/helpers/strings/string_helpers.c \
		src/helpers/strings/ft_strtok.c \
		src/helpers/parsing/parser.c \
		src/utils/sorting/merge.c \
		src/exec/exec.c \
		src/utils/free.c \
		src/helpers/env/env.c \
		src/helpers/expansion/token_helpers.c \
		src/helpers/expansion/expansion_helpers2.c \
		src/helpers/expansion/expansion_helpers.c \
		src/helpers/expansion/expand.c

# Object files
OBJS_DIR = build/
OBJS = $(SRC:src/%.c=$(OBJS_DIR)%.o)

# Include directories
INCLUDES =	-I ./includes \
			-I ./includes/libft/

# Targets
all: $(NAME)

$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT)
	@echo "$(CYAN)Linking $@...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@ $(LDFLAGS)
	@echo "$(GREEN)$@ is ready!$(RESET)"

$(LIBFT):
	@echo "$(MAGENTA)Building libft...$(RESET)"
	@make -C $(LIBFT_DIR)
	@echo "$(MAGENTA)Libft built successfully!$(RESET)"

clean:
	@echo "$(RED)Cleaning up object files...$(RESET)"
	@rm -rf $(OBJS_DIR)
	@make -C $(LIBFT_DIR) clean
	@echo "$(RED)Object files removed.$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME) and libft...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(RED)$(NAME) and libft removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
