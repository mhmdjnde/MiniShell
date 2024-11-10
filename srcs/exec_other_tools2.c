/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_other_tools2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 02:38:20 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 02:38:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_in_chek_absolute_dir(char **args, int f, int *exit_status)
{
	write(2, "bash: ", 6);
	without_quotes(args[0], 0);
	write(2, ": Is a directory\n", 17);
	if (f != 1)
		*exit_status = 126;
	return (0);
}

int	print_in_chek_absolute_denied(char **args, int f, int *exit_status)
{
	write(2, "bash: ", 6);
	without_quotes(args[0], 0);
	write(2, ": Permission denied\n", 20);
	if (f != 1)
		*exit_status = 126;
	return (-1);
}

int	print_in_chek_absolute_no_file(char **args)
{
	write(2, "bash: ", 6);
	without_quotes(args[0], 0);
	write(2, ": No such file or directory\n", 28);
	return (-1);
}

void	free_2charpointers(char *dir, char	*cmd_path)
{
	free(dir);
	free(cmd_path);
}

void	print_error(char *error)
{
	write(2, error, ft_strlen(error));
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
}
