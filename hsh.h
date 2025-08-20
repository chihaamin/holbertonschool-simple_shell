#ifndef HSH_H
#define HSH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Function prototypes */
char **tokenize_input(char *input);
char *find_command_path(char *command, char **envp);
int execute_command(char **args, char **envp, char *shell_name, int count);
int handle_builtins(char **args, char **envp);
void print_error(char *command, char *shell_name, int count);
int process_input(char *line, char **envp, char *shell_name, int count);
ssize_t read_input(char **line, size_t *len);

#endif
