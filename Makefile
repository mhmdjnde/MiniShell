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
