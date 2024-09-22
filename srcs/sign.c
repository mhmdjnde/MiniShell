/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sign.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 19:07:03 by mjoundi           #+#    #+#             */
/*   Updated: 2024/09/21 00:06:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct sigaction	g_sa;

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signals(void)
{
	g_sa.sa_handler = handle_sigint;
	sigemptyset(&g_sa.sa_mask);
	g_sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &g_sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	ignore_signals(void)
{
	g_sa.sa_handler = SIG_IGN;
	sigemptyset(&g_sa.sa_mask);
	g_sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &g_sa, NULL);
}

void	restore_signals(void)
{
	g_sa.sa_handler = handle_sigint;
	sigemptyset(&g_sa.sa_mask);
	g_sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &g_sa, NULL);
}
