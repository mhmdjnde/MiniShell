/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 22:27:55 by marvin            #+#    #+#             */
/*   Updated: 2024/09/10 22:27:55 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//return the index of the last herdoc
int	last_hd_index(t_redtools *red)
{
	int	i;
	int	l;

	i = 0;
	l = -1;
	while (red[i].file != NULL)
	{
		if (red[i].o == 4)
			l = i;
		i++;
	}
	return (l);
}

//functionality of not the last heredoc
void	func_hd_nl(char	*hd, char **en)
{
	char	*str;

	while (1)
	{
		str = readline("> ");
		if (!str)
			break ;
		if (*str)
			add_history(str);
		if (ft_strcmp(str, hd) == 0)
		{
			free(str);
			break ;
		}
		str = rm_dl(str);
		var_in_env(&str, en, 0);
		if (str)
			free(str);
	}
}

//functionality of thhe last heredoc
void	func_hd_l(int fd, char	**en, char *hd)
{
	char	*str;

	while (1)
	{
		str = readline("> ");
		if (!str)
			break ;
		if (*str)
			add_history(str);
		str = rm_dl(str);
		if (ft_strcmp(str, hd) == 0)
		{
			free(str);
			break ;
		}
		var_in_env(&str, en, 0);
		ft_putendl_fd(str, fd);
		if (str)
			free(str);
	}
}

void	func_nl(int l, t_redtools **red, char **en, int *i)
{
	while ((*red)[*i].file != NULL)
	{
		if (*i == l)
			break ;
		if ((*red)[*i].o == 4)
		{
			func_hd_nl((*red)[*i].file, en);
			(*red)[*i].o = 0;
		}
		(*i)++;
	}
}

//main functionality of the herdoc
char	*heredoc(t_redtools **red, char **en)
{
	int		i;
	char	*tmp;
	int		fd;
	int		l;

	i = 0;
	tmp = NULL;
	l = last_hd_index(*red);
	if (l != -1)
	{
		func_nl(l, red, en, &i);
		if (i == l)
		{
			tmp = tmp_file_gen();
			fd = open(tmp, O_CREAT | O_WRONLY | O_APPEND, 0777);
			func_hd_l(fd, en, (*red)[l].file);
			(*red)[l].o = 2;
			free((*red)[l].file);
			(*red)[l].file = tmp;
			close(fd);
		}
	}
	if (tmp)
		return (ft_strdup(tmp));
	return (NULL);
}
