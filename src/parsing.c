/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haitkadi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/26 22:27:20 by haitkadi          #+#    #+#             */
/*   Updated: 2022/02/26 22:27:24 by haitkadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static	void	free_paths(char **paths)
{
	int	i;

	i = 0;
	if (paths)
	{
		while (paths[i])
			free(paths[i++]);
		free(paths);
	}
}

static	char	**get_paths(char **env)
{
	char	*path;
	int		i;

	path = NULL;
	i = 0;
	while (env[i])
	{
		path = ft_strnstr(env[i], "PATH", 5);
		if (path)
		{
			return (ft_split(*&path + 5, ':'));
		}
		i++;
	}
	return (NULL);
}

static	char	*check_cmd(char **paths, char *cmd)
{
	char	*tmp_cmd;
	char	*full_cmd;
	int		i;

	i = 0;
	tmp_cmd = ft_strjoin("/", cmd);
	if (!tmp_cmd)
		return (NULL);
	while (paths[i])
	{
		full_cmd = ft_strjoin(paths[i], tmp_cmd);
		if (access(full_cmd, F_OK) == 0)
			break ;
		if (full_cmd)
			free(full_cmd);
		full_cmd = NULL;
		i++;
	}
	if (tmp_cmd)
		free(tmp_cmd);
	if (access(full_cmd, F_OK) == 0)
		return (full_cmd);
	return (NULL);
}

static	int	check_files(char **av, t_exec *exec)
{
	exec->infile = open(av[1], O_RDONLY);
	if (exec->infile == -1)
		return (exec->err = 1, 1);
	exec->outfile = open(av[4], O_CREAT | O_TRUNC | O_RDWR, 0777);
	if (exec->outfile == -1)
		return (exec->err = 4, 1);
	return (0);
}

t_exec	*parsing(char **env, char **av)
{
	t_exec	*exec;
	char	**paths;

	paths = NULL;
	exec = (t_exec *)ft_calloc(1, sizeof(t_exec));
	if (!exec)
		exit_err();
	if (check_files(av, exec))
		return (exec);
	paths = get_paths(env);
	if (!paths)
		return (exec->err = 9, exec);
	exec->cmd_switchs1 = ft_split(av[2], ' ');
	exec->cmd_switchs2 = ft_split(av[3], ' ');
	if (!exec->cmd_switchs1[0] || !exec->cmd_switchs2[0])
		return (exec->err = 5, exec);
	exec->full_cmd1 = check_cmd(paths, exec->cmd_switchs1[0]);
	if (!exec->full_cmd1)
		return (exec->err = 2, exec);
	exec->full_cmd2 = check_cmd(paths, exec->cmd_switchs2[0]);
	if (!exec->full_cmd2)
		return (exec->err = 3, exec);
	if (paths)
		free_paths(paths);
	return (exec);
}
