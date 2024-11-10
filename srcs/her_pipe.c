/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   her_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 04:13:08 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 04:13:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	her_run(char **str, t_tmptools *tmp, char **en, t_maintools *t)
{
	t_redtools	*red;
	int			i;

	i = 0;
	red = red_after_cmd(str, &t->exit_status);
	while (red != NULL && red[i].file != NULL)
	{
		tmp->tmp = red[i].file;
		red[i].file = without_quotes_ret(tmp->tmp, 0);
		i++;
		free(tmp->tmp);
	}
	if (red == NULL)
		return ;
	tmp->tmp = heredoc(&red, en);
	free_red(red);
}

void	heredoc_before_err(t_maintools *t, int err_i, char ***tmps)
{
	int		i;
	char	**temp;

	i = 0;
	*tmps = (char **)malloc(1 * sizeof(char *));
	(*tmps)[0] = NULL;
	temp = env_copy(t->cmds);
	while (i < err_i && temp[i] != NULL)
	{
		her_run(&temp[i], &t->tmp, t->en, t);
		if (t->tmp.tmp)
		{
			replace_heredoc_with_file(&t->cmds[i], t->tmp.tmp);
			*tmps = export_enc(*tmps, t->tmp.tmp);
			free(t->tmp.tmp);
			t->tmp.tmp = NULL;
		}
		i++;
	}
	free_args(&temp);
}

void	her_pipe(t_maintools *t, char ***tmps)
{
	int		i;
	char	**temp;

	i = 0;
	temp = env_copy(t->cmds);
	*tmps = (char **)malloc(1 * sizeof(char *));
	(*tmps)[0] = NULL;
	while (temp[i] != NULL)
	{
		her_run(&temp[i], &t->tmp, t->en, t);
		if (t->tmp.tmp)
		{
			replace_heredoc_with_file(&t->cmds[i], t->tmp.tmp);
			*tmps = export_enc(*tmps, t->tmp.tmp);
			free(t->tmp.tmp);
			t->tmp.tmp = NULL;
		}
		i++;
	}
	free_args(&temp);
}

int	check_token_err(char **cmds, t_maintools *t, char ***tmps)
{
	t_redtools	*red;
	char		*temp;
	int			i;
	int			es;

	i = 0;
	es = 0;
	while (cmds[i] != NULL)
	{
		temp = ft_strdup(cmds[i]);
		red = red_after_cmd(&temp, &es);
		if (red == NULL)
		{
			heredoc_before_err(t, i, tmps);
			return (free(temp), 0);
		}
		free(temp);
		free_red(red);
		i++;
	}
	return (1);
}
