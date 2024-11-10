/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invalid_cwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 01:24:59 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 01:24:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//the part of the no directory case where we remove the last dirs
void	rm_dr_helper(char **new_path, char *path, int *i, int *dd_len)
{
	(*new_path) = ft_strdup(path);
	*i = ft_strlen((*new_path)) - 1;
	*dd_len = *dd_len * 2;
	while (*dd_len > 0 && *i >= 0)
	{
		if ((*new_path)[*i] == '/')
		{
			(*i)--;
			continue ;
		}
		while (*i >= 0 && (*new_path)[*i] != '/')
			(*i)--;
		(*dd_len)--;
	}
	while (*i >= 0 && (*new_path)[*i] == '/')
		(*i)--;
	(*new_path)[*i + 1] = '\0';
}

//pt 2
char	*remove_last_dirs(char *path)
{
	int		len;
	int		i;
	int		dd_len;
	char	*new_path;

	len = ft_strlen(path);
	i = len - 1;
	dd_len = 0;
	while (i > 1)
	{
		if (path[i] == '.' && path[i - 1] == '.' && path[i - 2] == '/')
		{
			dd_len++;
			i -= 3;
		}
		else if (path[i] == '/')
			i--;
		else
			break ;
	}
	if (dd_len == 0)
		return (ft_strdup(path));
	rm_dr_helper(&new_path, path, &i, &dd_len);
	return (new_path);
}

//this function is for the case where we do "cd .." and 
//we are in a deleted directory, then we need to remove the
//last directories the number of times we did "cd .."
//to check if we did reach a true path
//pt 1
char	*rm_last_dir(char *path)
{
	char	*last_slash;
	char	*new_path;
	char	*concat_path;

	new_path = remove_last_dirs(path);
	if (!new_path)
		return (NULL);
	last_slash = ft_strrchr(new_path, '/');
	if (last_slash)
		*last_slash = '\0';
	if (access(new_path, F_OK) == 0 || ft_strlen(new_path) == 0)
		return (free(new_path), ft_strdup(path));
	else
	{
		free(new_path);
		printf("cd: error retrieving current directory: getcwd: ");
		printf("cannot access parent directories: No such file or directory\n");
		concat_path = malloc(ft_strlen(path) + 4);
		if (!concat_path)
			return (NULL);
		ft_strcpy(concat_path, path);
		ft_strcat(concat_path, "/..");
		return (concat_path);
	}
}
