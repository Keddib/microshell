#include "microshell.h"

int cd(const t_data *data)
{
	if (data->index != 2)
	{
		write(CERR, ERR_ARG, ft_strlen(ERR_ARG));
		return 1;
	}
	if (chdir(data->cmds[1]) == -1)
	{
		write(CERR, CANT_CHAN, ft_strlen(CANT_CHAN));
		write(CERR, data->cmds[1], ft_strlen(data->cmds[1]));
		write(CERR, "\n", 1);
		return 1;
	}
	return 0;
}

pid_t excute_cmd(t_data *data, char **envp)
{
	pid_t pid;
	if (pipe(data->fds) == -1)
		return fataErr();
	pid = fork();
	if (pid == -1)
		return fataErr();
	else if (pid == 0) // child
	{
		if (data->is_pipe)
			if (dup2(data->fds[1], COUT) == -1)
				return fataErr();
		if (data->is_prev_pipe)
		{
			if (dup2(data->tmpfd, CIN) == -1)
				return fataErr();
			if (close(data->tmpfd) == -1)
				return fataErr();
		}
		if (close(data->fds[1]) == -1)
			return fataErr();
		if (close(data->fds[0]) == -1)
			return fataErr();
		execve(data->cmds[0], data->cmds, envp);
		write(CERR, EXECVE_ERR, ft_strlen(EXECVE_ERR));
		write(CERR, data->cmds[0], ft_strlen(data->cmds[0]));
		write(CERR, "\n", 1);
		exit(0);
	}
	if(data->tmpfd != -1)
		if (close(data->tmpfd) == -1)
			return fataErr();
	if (close(data->fds[1]) == -1)
		return fataErr();
	data->tmpfd = dup(data->fds[0]);
	if (data->tmpfd == -1)
		return fataErr();
	if (close(data->fds[0]) == -1)
		return fataErr();
	return pid;
}


int main(int argc, char **argv, char **envp)
{
	if (argc < 2)
		return 0;
	argv++;

	t_data data;
	pid_t pid;
	int i = 0;
	int status;
	set_dataZero(&data);
	while (argv[i])
	{
		while (argv[i] && is_cmd(&data, argv[i]))
		{
			data.cmds[data.index++] = argv[i];
			i++;
		}
		data.cmds[data.index] = NULL;
		if (data.is_cd)
		{
			if (cd(&data))
				return 1;
		}
		else if (data.cmds[0])
			pid = excute_cmd(&data, envp);
		if (data.is_end)
		{
			waitpid(pid, &status, 0);
			data.is_end = 0;
		}
		if (argv[i])
			i++;
		data.index = 0;
	}
	if (data.tmpfd != -1)
		close(data.tmpfd);
	if(!data.is_end)
		pid = waitpid(pid, &status, 0);
	while (pid > 0)
		pid = waitpid(-1, NULL, 0);

	return status;
}
