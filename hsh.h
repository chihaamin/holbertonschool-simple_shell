#ifndef HSH_H
#define HSH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/* Function prototypes */
char **tokenize_input(char *input);
char *find_command_path(char *command);
int execute_command(char **args);
int handle_builtins(char **args);
void print_error(char *command, int count);

#endif
