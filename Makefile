NAME		=	minishell

HEADER		=	./includes

CC			=	gcc

CFLAGS		=	-Werror -Wall -Wextra -g -I $(HEADER)

LDFLAGS		=	-lreadline

SRCS		=	srcs/count_args.c \
				srcs/cwd.c\
				srcs/dollar.c\
				srcs/echo.c\
				srcs/env.c\
				srcs/exec_other.c\
				srcs/exit_args.c\
				srcs/export.c\
				srcs/export2.c\
				srcs/export3.c\
				srcs/ft_itoa.c\
				srcs/ft_substr.c\
				srcs/herdoc.c\
				srcs/main_utils.c\
				srcs/minishell.c\
				srcs/parse_args.c\
				srcs/parse_cmd.c\
				srcs/parse_flags.c\
				srcs/redi.c\
				srcs/sign.c\
				srcs/unset.c\
				srcs/utils.c\
				srcs/utils2.c\
				srcs/utils3.c\
				srcs/utils4.c\
				srcs/utils5.c\
				srcs/pipe.c\
				srcs/edit_her.c\
				srcs/pwd.c \
				srcs/oldpwd.c \
				srcs/env_var.c \
				srcs/var_ret.c \
				srcs/exec_other_tools1.c \
				srcs/exec_other_tools2.c \
				srcs/exec_get_cmd.c \
				srcs/utils6.c \
				srcs/export1.c \
				srcs/export_tools.c \
				srcs/her_red_tools.c \
				srcs/exit.c \
				srcs/built_ins.c \
				srcs/cd.c \
				srcs/ini_tools.c \
				srcs/events.c \
				srcs/clean.c \
				srcs/parse_cmd2.c \
				srcs/ini_check_pipes.c \
				srcs/pipe_cmds.c \
				srcs/her_pipe.c \
				srcs/red_check.c \
				srcs/red_func.c \
				srcs/red_clean.c \
				srcs/red_parse.c \
				srcs/pipe_func.c \
				srcs/unset_tools.c \
				srcs/unset_func.c \
				srcs/utils7.c \
				srcs/utils8.c \
				srcs/var_name_check.c \
				srcs/invalid_cwd.c

OBJS		=	$(SRCS:.c=.o)

all			:	$(NAME)

$(NAME)		:	$(OBJS) $(HEADER)
				$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

clean		:
				rm -rf $(OBJS)

fclean		:	clean
				rm -rf $(NAME)

re			:	fclean all

v			:	$(NAME)
				valgrind --leak-check=full ./$(NAME)
