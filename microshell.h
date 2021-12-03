#ifndef MICROSHELL_H
#define MICROSHELL_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CIN		0
#define COUT	1
#define CERR	2
#define ERR_ARG "error: cd: bad arguments\n"
#define CANT_CHAN "error: cd: cannot change directory to "
#define FATAL "error: fatal\n"
#define EXECVE_ERR "error: cannot execute "

typedef struct s_data
{
	char *cmds[64];
	size_t index;
	bool is_pipe;
	bool is_prev_pipe;
	bool is_end;
	bool is_cd;
	int fds[2];
	int tmpfd;
} t_data;



size_t ft_strlen(char *str);
int is_cmd(t_data *data, char *arg);
int fataErr();
void set_dataZero(t_data *data);


#endif
