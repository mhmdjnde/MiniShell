/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fdahouk <fdahouk@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:57:50 by fdahouk           #+#    #+#             */
/*   Updated: 2024/09/05 16:57:50 by fdahouk          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/stat.h>

extern int	g_s;

typedef struct s_redtools
{
	char	*file;
	int		o;
}	t_redtools;

typedef struct s_tmptools
{
	char	*tmp;
	int		*fd;
}	t_tmptools;

typedef struct s_maintools
{
	char		*str;
	char		**strs;
	char		**en;
	char		**ex;
	char		*cd;
	char		*temp;
	int			f;
	int			out;
	int			in;
	int			*fd;
	t_tmptools	tmp;
	int			exit_status;
	char		**cmds;
	int			cdf;
	int			pf;
}	t_maintools;

typedef struct s_redmain
{
	int			*tab;
	char		q;
	int			rf;
	int			check;
	t_redtools	*red;
}	t_redmain;

typedef struct s_check_ve
{
	int		flag;
	int		result;
	int		i;
	int		f;
	int		dir_len;
	int		status;
	char	*path;
	char	*dir;
	char	*cmd_path;
	char	**args;
	char	**env;
}	t_check_ve;

typedef struct s_pipetools
{
	int		i;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	int		num_cmds;
	pid_t	*pids;
	char	**tmps;
}	t_pipetools;

int			ft_strlen(char *str);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strcat(char *dest, char *src);
char		*ft_strrchr(char *s, int c);
char		*ft_strncat(char *dest, char *src, unsigned int nb);
int			valquotes(char *str);
int			parse_cmd(char *str, char *cmd);
int			count_args(char *str, char *cmd);
int			get_index(char *str);
int			parse_flags(char *str, char *flag);
int			ft_strncmp(char *s1, char *s2, int n);
int			is_ok(char c);
int			check_equal(char *str);
int			ret_s_index(char *str, char **env);
int			is_ok(char c);
int			is_ok2(char c);
int			get_pid(void);
int			ft_atoi(const char *nptr);
int			check_ve(char **args, char **env, int f, int *exit_status);
char		*ft_substr(char *s, int start, int len);
char		*get_cmd(char *str);
void		edit_pwd(char ***ex, char ***en, t_maintools *tools);
void		inc_shlvl(char ***ex, char ***en);
void		handle_sigint(int sig);
char		**remove_at_s(char **env, int index);
void		ft_strncpy(char *dest, char *src, int n);
void		find_arg_bounds(char *str, int *index, char **start, int *len);
char		*extract_arg(char *str, int *index);
char		**parse_args(char *str, char *cmd);
void		exit_args_check(char **args, int *exit_status);
void		free_args(char ***args);
void		echo_args_check(char **strs, t_maintools *tools);
void		q_args(char **args);
void		without_quotes(char *str, int flag);
void		print_env(char **env);
char		*ft_strcpy(char *dest, char *src);
void		var_in_env(char **str, char **env, int *exit_status);
char		*without_quotes_ret(char *str, int flag);
char		*rm_dl(char *str);
char		*rm_bs(char *str);
void		print_exp(char **ex);
char		**env_copy(char **env);
void		sort_env(char **arr, int n);
void		get_pwd(char *str, int *exit_status, char **ex);
char		*env_search(char *str, char **env);
void		add_exp(t_maintools *tools, char ***ex, char ***en, int sf);
void		do_cd(char **env, char *str, char **old, t_maintools *tools);
char		*add_equal(char *str);
void		rm_exp(char **args, char ***ex, char ***en, int *exit_status);
char		*ret_to_equal(char *str);
void		free_en(char **args);
char		*add_quotes(char *str);
void		edit_str(char **str, int start, int end, char *env_value);
char		*var_ret(char *str, int *i);
char		*ft_strdup(char *src);
char		*ft_itoa(int n);
char		**export_enc(char **env, char *var);
int			redout_check(char **rediles);
int			redin_check(char **rediles, int rfd);
void		red_rem(char **strs, char **rediles);
int			rederror(char **rediles);
char		**parse_files(char **strs);
t_redtools	*red_after_cmd(char **str, int *exit_status);
int			*func_red(t_redtools *red, int *exit_status);
void		free_red(t_redtools *red);
void		ft_putendl_fd(char *s, int fd);
void		ft_putstr_fd(char *s, int fd);
char		*add_ch(char *str, char c);
char		*heredoc(t_redtools **red, char **en);
void		q_red(t_redtools **red);
void		free_in_error(char **new, int *i);
void		exp_q(char **ex);
int			empty(char *str);
void		q_args(char **args);
void		q_args2(char **args);
int			*red_run(char **str, t_tmptools *tmp, char **en, t_maintools *t);
int			fix_after_red(int in, int out, int *fd);
void		del_temp(char *tp, char **en);
int			args_len(char **strs);
//  exec_other functions end 
int			pipe_check(char *str);
int			run_one_cmd(t_maintools *tools);
long long	ft_atol(const char *nptr);
void		fill_status(char **args);
void		setup_signals(void);
void		ignore_signals(void);
void		restore_signals(void);
void		check_if_minus(t_maintools *tools, int tab[2]);
int			*init_int_tab(void);
void		print_cr(char cr);
int			edit_red_ext(int *exit_status);
int			*init_rtab(void);
void		free_three(char *t, char *t2, char *t3);
void		run_pipes(t_maintools *tools);
char		*get_pwd2(char **env, int	*exit_status);
void		edit_oldpwd(char ***ex, char ***en, t_maintools *tools);
void		add_pwd(char ***ex, char ***en);
void		replace_heredoc_with_file(char **input, char *replacement);

#endif
