NAME		=	minishell
HEADER		=	./includes
CC			=	gcc
CFLAGS		=	-Werror -Wall -Wextra -g -I $(HEADER)
LDFLAGS		=	-lreadline

# Colors
RESET		=	\033[0m
GREEN		=	\033[1;32m
YELLOW		=	\033[1;33m
BLUE		=	\033[1;34m
CYAN		=	\033[1;36m
RED			=	\033[1;31m
MAGENTA		=	\033[1;35m

# Messages
MSG_BUILD	=	"$(GREEN)Building $(NAME)...$(RESET)"
MSG_CLEAN	=	"$(YELLOW)Cleaning object files...$(RESET)"
MSG_FCLEAN	=	"$(RED)Removing $(NAME)...$(RESET)"
MSG_REBUILD	=	"$(MAGENTA)Rebuilding everything!$(RESET)"
MSG_VALGRIND	=	"$(CYAN)Running Valgrind for leak checks...$(RESET)"
MSG_COMPLETE	=	"$(GREEN)Build complete!$(RESET)"

SRCS		=	srcs/parsing/count_args.c \
				srcs/built_ins/cwd.c\
				srcs/parsing/dollar.c\
				srcs/built_ins/echo.c\
				srcs/built_ins/env.c\
				srcs/built_ins/exec_other.c\
				srcs/parsing/exit_args.c\
				srcs/built_ins/export.c\
				srcs/built_ins/export2.c\
				srcs/built_ins/export3.c\
				srcs/utils_and_sig/ft_itoa.c\
				srcs/utils_and_sig/ft_substr.c\
				srcs/redirection/herdoc.c\
				srcs/utils_and_sig/main_utils.c\
				srcs/main.c\
				srcs/parsing/parse_args.c\
				srcs/parsing/parse_cmd.c\
				srcs/parsing/parse_flags.c\
				srcs/redirection/redi.c\
				srcs/utils_and_sig/sign.c\
				srcs/built_ins/unset.c\
				srcs/utils_and_sig/utils.c\
				srcs/utils_and_sig/utils2.c\
				srcs/utils_and_sig/utils3.c\
				srcs/utils_and_sig/utils4.c\
				srcs/utils_and_sig/utils5.c\
				srcs/pipe/pipe.c\
				srcs/redirection/edit_her.c\
				srcs/built_ins/pwd.c \
				srcs/parsing/oldpwd.c \
				srcs/parsing/env_var.c \
				srcs/pipe/var_ret.c \
				srcs/built_ins/exec_other_tools1.c \
				srcs/built_ins/exec_other_tools2.c \
				srcs/built_ins/exec_get_cmd.c \
				srcs/utils_and_sig/utils6.c \
				srcs/built_ins/export1.c \
				srcs/utils_and_sig/export_tools.c \
				srcs/redirection/her_red_tools.c \
				srcs/built_ins/exit.c \
				srcs/built_ins/built_ins.c \
				srcs/built_ins/cd.c \
				srcs/utils_and_sig/ini_tools.c \
				srcs/pipe/ini_check_pipes.c \
				srcs/pipe/pipe_cmds.c \
				srcs/redirection/red_check.c \
				srcs/redirection/red_func.c \
				srcs/redirection/red_clean.c \
				srcs/redirection/red_parse.c \
				srcs/pipe/pipe_func.c \
				srcs/built_ins/unset_tools.c \
				srcs/built_ins/unset_func.c \
				srcs/utils_and_sig/utils7.c \
				srcs/utils_and_sig/utils8.c \
				srcs/pipe/var_name_check.c \
				srcs/parsing/invalid_cwd.c \
				srcs/parsing/parse_cmd2.c \
				srcs/parsing/events.c \
				srcs/utils_and_sig/clean.c \
				srcs/pipe/her_pipe.c

OBJS		=	$(SRCS:.c=.o)

all			:	$(NAME)

%.o			:	%.c
				@echo "$(RED)Compiling $<...$(RESET)"
				@$(CC) $(CFLAGS) -c $< -o $@

$(NAME)		:	$(OBJS) $(HEADER)
				@echo $(MSG_BUILD)
				@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
				@echo $(MSG_COMPLETE)

clean		:
				@echo $(MSG_CLEAN)
				@rm -rf $(OBJS)

fclean		:	clean
				@echo $(MSG_FCLEAN)
				@rm -rf $(NAME)

re			:	fclean all
				@echo $(MSG_REBUILD)

v			:	$(NAME)
				@echo $(MSG_VALGRIND)
				@valgrind -q --leak-check=full ./$(NAME)
