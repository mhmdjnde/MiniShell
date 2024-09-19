/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 17:10:31 by marvin            #+#    #+#             */
/*   Updated: 2024/09/06 17:10:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redname_check(char *str, int i)
{
	int	j;

	j = i;
	while (str[j] != '\0' && str[j] == ' ')
		j++;
	if (str[j] == '\0' || str[j] == '>' || str[j] == '<')
		return (0);
	else
		return (1);
}

int	check_red1(char *str, int i, char cr, char c)
{
	if (redname_check(str, i) == 0)
	{
		printf("syntax error near %c\n", cr);
		return (-4);
	}
	if ((cr == '>' && str[i] == '<') || (cr == '<' && str[i] == '>'))
	{
		printf("syntax error near %c\n", cr);
		return (-1);
	}
	if (c == 1 && str[i] != '\0')
	{
		if (cr == '>')
			return (1);
		if (cr == '<')
			return (2);
	}
	if (c == 2 && str[i] != '\0')
	{
		if (cr == '>')
			return (3);
		if (cr == '<')
			return (4);
	}
	return (10);
}

int	check_red2(char *str, int i, char cr, char c)
{
	if (c == 3)
	{
		printf("syntax error near %c\n", cr);
		return (-2);
	}
	if (c > 3)
	{
		printf("syntax error near %c%c\n", cr, cr);
		return (-3);
	}
	if (str[i] == '\0')
	{
		printf("syntax error near 'newline'\n");
		return (0);
	}
	return (10);
}

int	check_red(char *str)
{
	int		c;
	int		i;
	char	cr;
	int		t;

	i = 0;
	c = 0;
	cr = str[i];
	while (str[i] != '\0' && str[i] == cr)
	{
		i++;
		c++;
	}
	t = check_red1(str, i, cr, c);
	if (t != 10)
		return (t);
	t = check_red2(str, i, cr, c);
	if (t != 10)
		return (t);
	return (0);
}

int	red_count(char *str)
{
	int	c;
	int	i;

	i = 0;
	c = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '>' || str[i] == '<')
			c++;
		i++;
	}
	return (c);
}

void	free_red(t_redtools *red)
{
	int	i;

	i = 0;
	while (red[i].file != NULL)
	{
		free(red[i].file);
		i++;
	}
	free(red);
}

char	*del_part(char *str, int st, int len)
{
	int		l;
	int		nl;
	char	*new_str;

	l = ft_strlen(str);
	if (st < 0 || st >= l || len <= 0 || st + len > l)
		return (str);
	nl = l - len + 1;
	new_str = (char *)malloc((nl + 2) * sizeof(char));
	if (!new_str)
		return (NULL);
	ft_strncpy(new_str, str, st);
	new_str[st] = ' ';
	ft_strcpy(new_str + st + 1, str + st + len);
	free(str);
	new_str[nl] = '\0';
	return (new_str);
}

int	more_red_check(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '>' || str[i] == '<')
			return (1);
		i++;
	}
	return (0);
}

int	file_nl(t_redmain *r, char **str)
{
	if (r->rf == 0)
	{
		r->red[r->tab[2]].file = ft_substr((*str),
				r->tab[1], r->tab[0] - r->tab[1]);
		r->tab[1]--;
		while ((*str)[r->tab[1]] == ' ' && r->tab[1] >= 0)
			r->tab[1]--;
		if (r->check == 3 || r->check == 4)
			r->tab[1]--;
		(*str) = del_part((*str), r->tab[1], r->tab[0] - r->tab[1]);
		r->red[r->tab[2]].o = r->check;
		r->tab[0] = r->tab[1] + 1;
		r->tab[2]++;
		if (more_red_check((*str) + r->tab[0]) == 0)
		{
			r->red[r->tab[2]].file = NULL;
			return (0);
		}
		r->rf = 1;
		while ((*str)[r->tab[0]] == ' ')
			r->tab[0]++;
	}
	return (1);
}

int	file_l(t_redmain *r, char **str)
{
	if ((*str)[r->tab[0]] == '\0')
	{
		r->rf = 0;
		r->red[r->tab[2]].file = ft_substr((*str),
				r->tab[1], r->tab[0] - r->tab[1]);
		r->tab[1]--;
		while ((*str)[r->tab[1]] == ' ' && r->tab[1] >= 0)
			r->tab[1]--;
		if (r->check == 3 || r->check == 4)
			r->tab[1]--;
		(*str) = del_part((*str), r->tab[1], r->tab[0] - r->tab[1]);
		r->red[r->tab[2]].o = r->check;
		r->tab[0] = r->tab[1] + 1;
		r->tab[2]++;
		return (0);
	}
	return (1);
}

int	red_loop(t_redmain *r, char **str)
{
	r->rf = 1;
	r->check = check_red((*str) + r->tab[0]);
	if (r->check <= 0)
		return (0);
	if (r->check == 1 || r->check == 2)
		r->tab[0]++;
	else
		r->tab[0] += 2;
	while ((*str)[r->tab[0]] != '\0' && (*str)[r->tab[0]] == ' ')
		r->tab[0]++;
	r->tab[1] = r->tab[0];
	while ((*str)[r->tab[0]] != '\0' && (*str)[r->tab[0]] != '>'
		&& (*str)[r->tab[0]] != '<' && (*str)[r->tab[0]] != '\''
		&& (*str)[r->tab[0]] != '"' && (*str)[r->tab[0]] != ' ')
		r->tab[0]++;
	if ((*str)[r->tab[0]] == '>' || (*str)[r->tab[0]] == '<'
		|| (*str)[r->tab[0]] == ' ')
	{
		r->rf = 0;
		r->tab[0]--;
		return (1);
	}
	return (2);
}

void	red_q(t_redmain *r, char **str)
{
	r->q = (*str)[r->tab[0]];
	r->tab[0]++;
	while ((*str)[r->tab[0]] != '\0' && (*str)[r->tab[0]] != r->q)
		r->tab[0]++;
	if ((*str)[r->tab[0]] == r->q)
		r->tab[0]++;
	if (r->rf == 1 && ((*str)[r->tab[0]] == '\0' || (*str)[r->tab[0]] == ' '
		|| (*str)[r->tab[0]] == '>' || (*str)[r->tab[0]] == '<'))
		r->rf = 0;
}

int	red_main_loop(t_redmain *r, char **str)
{
	int	t;

	if ((*str)[r->tab[0]] == '"' || (*str)[r->tab[0]] == '\'')
		red_q(r, str);
	else if ((*str)[r->tab[0]] == '>' || (*str)[r->tab[0]] == '<')
	{
		t = red_loop(r, str);
		if (t == 0)
			return (0);
		else if (t == 1)
			return (1);
		if (file_l(r, str) == 0)
			return (2);
		if ((*str)[r->tab[0]] == '\'' || (*str)[r->tab[0]] == '"')
			return (1);
	}
	else
	{
		if ((*str)[r->tab[0]] != '\0')
			r->tab[0]++;
		if ((*str)[r->tab[0]] == '\0' && r->rf == 1)
			r->rf = 0;
	}
	return (-1);
}

t_redtools	*red_after_cmd(char **str)
{
	t_redmain	r;
	int			t;

	r.tab = malloc(3 * sizeof(int));
	r.tab[0] = 0;
	r.tab[2] = 0;
	r.rf = -1;
	r.red = malloc((red_count(*str) + 1) * sizeof(t_redtools));
	while ((*str)[r.tab[0]] != '\0' && (*str)[r.tab[0]] == ' ')
		r.tab[0]++;
	while ((*str)[r.tab[0]] != '\0')
	{
		t = red_main_loop(&r, str);
		if (t == 0)
			return (free(r.tab), free(r.red), NULL);
		else if (t == 1)
			continue ;
		else if (t == 2)
			break ;
		if (file_nl(&r, str) == 0)
			return (free(r.tab), r.red);
	}
	r.red[r.tab[2]].file = NULL;
	return (free(r.tab), r.red);
}

void	q_red(t_redtools **red)
{
	int		i;
	char	*t;

	i = 0;
	while (red[i]->file != NULL)
	{
		t = red[i]->file;
		red[i]->file = without_quotes_ret(t, 0);
		i++;
		free(t);
	}
}

int	open_lin(int lin, int **fd, t_redtools *red)
{
	if (lin != -1)
	{
		if (red[lin].o == 1)
			(*fd)[0] = open(red[lin].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (red[lin].o == 3)
			(*fd)[0] = open(red[lin].file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if ((*fd)[0] < 0)
		{
			free((*fd));
			return (0);
		}
		if (dup2((*fd)[0], STDOUT_FILENO) < 0)
		{
			free((*fd));
			perror("dup2");
			return (0);
		}
	}
	return (1);
}

int	in_files(t_redtools *red, int **fd, int **inout, int i)
{
	if (red[i].o == 1 || red[i].o == 3)
	{
		if (red[i].o == 1)
			(*fd)[0] = open(red[i].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (red[i].o == 3)
			(*fd)[0] = open(red[i].file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if ((*fd)[0] < 0)
		{
			free((*fd));
			return (0);
		}
		(*inout)[0] = i;
		close((*fd)[0]);
	}
	return (1);
}

int	red_func_loop(t_redtools *red, int **fd, int *i, int **inout)
{
	int	t;

	t = in_files(red, fd, inout, *i);
	if (t == 0)
		return (0);
	if (red[*i].o == 2)
	{
		(*fd)[1] = open(red[*i].file, O_RDONLY);
		if ((*fd)[1] < 0)
		{
			printf("%s no such file or directory\n", red[*i].file);
			free((*fd));
			exit_status = 1;
			return (0);
		}
		(*inout)[1] = *i;
		close((*fd)[1]);
	}
	(*i)++;
	return (1);
}

int	open_lout(int *inout, int **fd, t_redtools *red)
{
	if (inout[1] != -1)
	{
		(*fd)[1] = open(red[inout[1]].file, O_RDONLY);
		if ((*fd)[1] < 0)
			return (0);
		if (dup2((*fd)[1], STDIN_FILENO) < 0)
		{
			free((*fd));
			perror("dup2");
			return (0);
		}
	}
	return (1);
}

int	*func_red(t_redtools *red)
{
	int	i;
	int	*inout;
	int	*fd;
	int	t;

	i = 0;
	inout = malloc(2 * sizeof(int));
	inout[0] = -1;
	inout[1] = -1;
	fd = malloc(2 * sizeof(int));
	fd[0] = -1;
	fd[1] = -1;
	while (red[i].file != NULL)
	{
		t = red_func_loop(red, &fd, &i, &inout);
		if (t == 0)
			return (free(inout), NULL);
	}
	t = open_lin(inout[0], &fd, red);
	if (t == 0)
		return (free(inout), NULL);
	t = open_lout(inout, &fd, red);
	if (t == 0)
		return (free(inout), NULL);
	return (free(inout), fd);
}

// int	main()
// {
// 	char *str = ft_strdup("echo jnde>asd<<<asdlr> jnde2");
// 	t_redtools *red;
// 	int *fds;
// 	int in, out;

// 	// Save original stdin and stdout file descriptors
// 	in = dup(STDIN_FILENO);
// 	out = dup(STDOUT_FILENO);

// 	// Parse the redirections from the string
// 	red = red_after_cmd(&str);
// 	if (red == NULL)
// 	{
// 		printf("Error parsing redirections\n");
// 		return (1);
// 	}
// 	printf("%s\n", str);
// 	// Handle the file redirections
// 	// fds = func_red(red);
// 	// if (fds == NULL)
// 	// {
// 	// 	printf("Error handling file redirections\n");
// 	// 	free_red(red);
// 	// 	return (1);
// 	// }

// 	// // Print the result of the redirections
// 	int i = 0;
// 	while (red[i].file != NULL)
// 	{
// 		// ft_putstr_fd("xx\n", fds[0]);
// 		printf("Redirection %d: File = %s,
//		Type = %d\n", i, red[i].file, red[i].o);
// 		i++;
// 	}

// 	// // Print something after redirections are done
// 	// ft_putstr_fd("DONE!\n", fds[0]);

// 	// // Restore the original stdin and stdout
// 	// if (dup2(in, STDIN_FILENO) < 0 || dup2(out, STDOUT_FILENO) < 0)
// 	// {
// 	// 	perror("dup2");
// 	// 	return (1);
// 	// }
// 	// printf("TEST!");
// 	// // Close the saved file descriptors
// 	// close(in);
// 	// close(out);
// 	// if (fds[0] >= 0)
// 	// 	close(fds[0]);
// 	// if (fds[1] >= 0)
// 	// 	close(fds[1]);
// 	// // Clean up
// 	// free_red(red);
// 	// free(fds);
// 	// return (0);
// }
