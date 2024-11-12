/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 23:31:31 by marvin            #+#    #+#             */
/*   Updated: 2024/11/10 23:31:31 by marvin           ###   ########.fr       */
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
	int			of;
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

/* Standard C library functions */
int			ft_strlen(char *str);
int			ft_strcmp(char *s1, char *s2);
int			ft_strncmp(char *s1, char *s2, int n);
char		*ft_strcat(char *dest, char *src);
char		*ft_strncat(char *dest, char *src, unsigned int nb);
char		*ft_strcpy(char *dest, char *src);
void		ft_strncpy(char *dest, char *src, int n);
char		*ft_strdup(char *src);
char		*ft_substr(char *s, int start, int len);
char		*ft_strrchr(char *s, int c);
char		*ft_itoa(int n);
long long	ft_atol(const char *nptr);
int			ft_atoi(const char *nptr);
void		ft_putendl_fd(char *s, int fd);
void		ft_putstr_fd(char *s, int fd);

/* String and command parsing functions */
int			valquotes(char *str);
int			parse_cmd(char *str, char *cmd);
int			count_args(char *str, char *cmd);
int			get_index(char *str);
int			parse_flags(char *str, char *flag);
int			is_ok(char c);
int			is_ok2(char c);
int			check_equal(char *str);
int			ret_s_index(char *str, char **env);
char		*get_cmd(char *str);

/* Environment variable handling functions */
void		edit_pwd(char ***ex, char ***en, t_maintools *tools);
void		inc_shlvl(char ***ex, char ***en);
char		**env_copy(char **env);
void		print_env(char **env);
void		print_exp(char **ex);
void		var_in_env(char **str, char **env, int *exit_status);
char		*env_search(char *str, char **env);
void		add_exp(t_maintools *tools, char ***ex, char ***en, int sf);
void		rm_exp(char **args, char ***ex, char ***en, int *exit_status);
char		**export_enc(char **env, char *var);
int			ret_s_index2(char *str, char **env);
int			ret_s_indexx(char *str, char **env);
char		*ret_to_equal(char *str);
char		*ret_to_equal2(char *str);
void		exp_q(char **ex);
void		add_pwd(char ***ex, char ***en);
void		edit_oldpwd(char ***ex, char ***en, t_maintools *tools);
void		add_pwd_2(char *pwd, t_maintools *tools);
char		*get_pwd2(char **env, int *exit_status);
void		get_pwd(char *str, int *exit_status, char **ex);
void		minuscase(char **old, int *exit_status, char **env);
char		*rm_last_dir(char *path);
char		*add_quotes(char *str);

/* Argument parsing and manipulation functions */
void		find_arg_bounds(char *str, int *index, char **start, int *len);
char		*extract_arg(char *str, int *index);
char		**parse_args(char *str, char *cmd);
void		exit_args_check(char **args, int *exit_status);
void		free_args(char ***args);
void		q_args(char **args);
void		q_args2(char **args);
void		echo_args_check(char **strs);
void		without_quotes(char *str, int flag);
char		*without_quotes_ret(char *str, int flag);
char		*rm_dl(char *str);
char		*rm_bs(char *str);
void		q_red(t_redtools **red);

/* Redirection handling functions */
int			redout_check(char **rediles);
int			redin_check(char **rediles, int rfd);
void		red_rem(char **strs, char **rediles);
int			rederror(char **rediles);
char		**parse_files(char **strs);
t_redtools	*red_after_cmd(char **str, int *exit_status);
int			*func_red(t_redtools *red, int *exit_status);
void		free_red(t_redtools *red);
char		*heredoc(t_redtools **red, char **en);
int			red_count(char *str);
int			check_red(char *str);
int			red_main_loop(t_redmain *r, char **str, int *exit_status);
char		*del_part(char *str, int st, int len);
int			more_red_check(char *str);
int			*init_rtab(void);
int			edit_red_ext(int *exit_status);
void		print_cr(char cr);
int			*init_int_tab(void);
int			file_nl(t_redmain *r, char **str);

/* Pipe handling functions */
int			pipe_check(char *str);
int			check_after_pipe(char *str);
int			pipe_count(char *str);
void		ini_pipe(int *i, int **tab, char *str, char ***cmds);
char		**parse_pipe(char *str);
void		her_pipe(t_maintools *t, char ***tmps);
void		her_run(char **str, t_tmptools *tmp, char **en, t_maintools *t);
int			check_token_err(char **cmds, t_maintools *t, char ***tmps);
void		init_pipe_tools(t_pipetools *pt, int num_cmds);
int			pipe_main(t_pipetools *pt, t_maintools *tools);

/* Signal handling functions */
void		handle_sigint(int sig);
void		setup_signals(void);
void		ignore_signals(void);
void		restore_signals(void);

/* Utility functions */
void		free_in_error(char **new, int *i);
int			empty(char *str);
int			args_len(char **strs);
void		free_en(char **args);
int			get_pid(void);
void		print_error(char *error);
void		free_2charpointers(char *dir, char *cmd_path);
int			fork_check(int pid);
int			execute_command(char *cmd_path, t_check_ve *vars, int *exit_status);
void		free_tools(t_maintools *tools);
void		clean(t_maintools *tools);
void		free_three(char *t, char *t2, char *t3);
char		*tmp_file_gen(void);
int			is_num(char *str);
int			event_check(char *str);
char		*add_equal(char *str);
void		del_temp(char *tp, char **en);
char		*add_ch(char *str, char c);

/* Command execution functions */
void		ex_st(char **args, int *exit_status);
void		exitt(t_maintools *tools);
void		export(t_maintools *tools);
void		echo(t_maintools *tools);
void		envv(char **env, int *exit_status);
int			cd(t_maintools *tools);
void		unset(t_maintools *tools);
int			run_one_cmd(t_maintools *tools);
void		run_pipes(t_maintools *tools);
int			check_ve(char **args, char **env, int f, int *exit_status);

/* Main loop and initialization */
void		ini_tools_1(t_maintools *tools, char **env);
int			ini_loop(t_maintools *tools);
int			check_before_run(t_maintools *t);

/* Parsing command and quote functions */
int			help_cmd(int **tab, char *str, char **temp);
int			parse_q(char *str, int **tab, char **temp);
int			parse_2q(int **tab, char *str, char **temp);
void		without_q(int *qf, int *i, char *str);
void		without_2q(int *qf, int *i, char *str);
void		skip_q(char *str, int *i, char c);
int			parse_q1(char *str, int *i);
int			parse_q2(char *str, int *i);
int			bs_check(char *str, char c);

/* Variable expansion and handling functions */
void		sign_check(char **args, int *i, int *sf);
int			p_exp_err(t_maintools *tools, int i, char *error, int ext);
void		check_if_minus(t_maintools *tools, int tab[2]);
void		edited_exit_status(t_maintools *tools, int f, int sf);
int			exp_arg_check(char *str);
void		func_add_exp(int i, char ***en, t_maintools *tools, char ***ex);
void		check_availble(int idx, char ***en, char **args, int i);
char		*var_ret_func(char *str, int *i, int *index);
char		*var_ret(char *str, int *i);

/* Additional execution helper functions */
// exec_other_tools
void		print_in_checkve(char **args, int f, int *exit_status);
void		init_vars2(t_check_ve *vars, int f, char **args, char **env);
void		calc_dir_lenth(t_check_ve *vars);
void		dup_into_dir(t_check_ve *vars);
void		increment_i(t_check_ve *vars);
int			print_in_chek_absolute_dir(char **args, int f, int *exit_status);
int			print_in_chek_absolute_denied(char **args, int f, int *exit_status);
int			print_in_chek_absolute_no_file(char **args);
void		do_cd(char **env, char *str, char **old, t_maintools *tools);

/* Unset related functions */
int			check_unset(char *str);
int			flag_check(char *str);
char		**ini_rem(int index, char **env, int *i, int *j);
char		**remove_at_s(char **env, int index);
void		free_three(char *t, char *t2, char *t3);
int			printing_with_return(int *exit_status);
void		inc_and_free(int *i, char *t3, char *t2, char *t);
void		sort_env(char **arr, int n);
int			*red_run(char **str, t_tmptools *tmp, char **en, t_maintools *t);
int			fix_after_red(int in, int out, int *fd);
void		replace_heredoc_with_file(char **input, char *replacement);

#endif
