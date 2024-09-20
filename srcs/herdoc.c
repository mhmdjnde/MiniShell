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

char	*tmp_file_gen(void)
{
	char	*str;

	str = ft_strdup("temp");
	while (access(str, F_OK) == 0)
	{
		str = add_ch(str, 'p');
	}
	return (str);
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

// int main(void)
// {
//     // Test case setup with multiple heredocs
//     t_redtools red[4] = {
//         {"STOP", 4},    // First heredoc with delimiter "STOP"
//         {"QUIT", 4},    // Second heredoc with delimiter "QUIT"
//         {"END", 4},     // Third heredoc with delimiter "END"
//         {NULL, 0}       // End of redirections
//     };
//     heredoc(red, NULL);

//     return 0;
// }
