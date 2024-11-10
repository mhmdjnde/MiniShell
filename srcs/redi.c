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
			ft_putstr_fd(red[*i].file, 2);
			write(2, ": No such file or directory\n", 28);
			free((*fd));
			return (1);
		}
		(*inout)[1] = *i;
		close((*fd)[1]);
	}
	(*i)++;
	return (0);
}

int	*func_red(t_redtools *red, int *exit_status)
{
	int	i;
	int	*inout;
	int	*fd;
	int	t;

	i = 0;
	inout = init_int_tab();
	fd = init_int_tab();
	while (red[i].file != NULL)
	{
		t = red_func_loop(red, &fd, &i, &inout);
		if (t == 1)
		{
			*exit_status = 1;
			return (free(inout), NULL);
		}
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
