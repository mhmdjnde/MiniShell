/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   her_red_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 03:14:05 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 03:14:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fix_after_red(int in, int out, int *fd)
{
	if (dup2(out, STDOUT_FILENO) == -1 || dup2(in, STDIN_FILENO) == -1)
	{
		perror("dup2");
		return (-1);
	}
	if (fd[1] > 0)
		close(fd[1]);
	if (fd[0] > 0)
		close(fd[0]);
	return (1);
}

void	del_temp(char *tp, char **en)
{
	char	**args;
	int		exit_status;

	exit_status = 0;
	if (tp != NULL)
	{
		args = malloc(3 * sizeof(char *));
		if (args == NULL)
			return ;
		args[0] = ft_strdup("rm");
		args[1] = ft_strdup(tp);
		args[2] = NULL;
		check_ve(args, en, 1, &exit_status);
		free_args(&args);
	}
}

//generate the temp file name
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
