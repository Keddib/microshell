#include "microshell.h"

size_t ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}


int is_cmd(t_data *data, char *arg)
{
	if (data->is_pipe)
	{
		data->is_prev_pipe = 1;
		data->is_pipe = 0;
	}
	if (!strcmp(arg, "|"))
	{
		data->is_pipe = 1;
		return 0;
	}
	if (!strcmp(arg, ";"))
	{
		data->is_end = 1;
		return 0;
	}
	if (!strcmp(arg, "cd"))
		data->is_cd = 1;
	return 1;
}


int fataErr()
{
	write(CERR, FATAL, ft_strlen(FATAL));
	exit(1);
	return 1;
}


void set_dataZero(t_data *data)
{
	data->cmds[0] = NULL;
	data->fds[0] = -1;
	data->fds[1] = -1;
	data->tmpfd = -1;
	data->index = 0;
	data->is_end = 0;
	data->is_pipe = 0;
	data->is_prev_pipe = 0;
	data->is_cd = 0;
}
