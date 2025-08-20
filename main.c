#include "hsh.h"

/**
 * read_input - Read input from stdin
 * @line: Pointer to buffer for storing input
 * @len: Pointer to size of buffer
 * Return: Number of characters read, or -1 on failure
 */
ssize_t read_input(char **line, size_t *len)
{
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "$ ", 2);

	return (getline(line, len, stdin));
}

/**
 * process_input - Process the input line
 * @line: Input line to process
 * @envp: Environment variables
 * @shell_name: Name of the shell
 * @count: Command count
 * @last_status: Pointer to the status of the last command
 * Return: Exit status of the command, or -1 if exit command
 */
int process_input(char *line, char **envp, char *shell_name,
				  int count, int *last_status)
{
	char **args;
	int status = 0;
	int builtin_result;

	args = tokenize_input(line);
	if (args[0] == NULL)
	{
		free(args);
		return (0);
	}

	builtin_result = handle_builtins(args, envp);
	if (builtin_result == 2)
	{
		free(args);
		return (-1);
	}
	else if (builtin_result == 1)
	{
		free(args);
		return (0);
	}

	status = execute_command(args, envp, shell_name, count);
	free(args);
	*last_status = status;
	return (status);
}

/**
 * main - Shell entry point
 * @argc: argument count
 * @argv: argument vector
 * @envp: environmental variables
 * Return: exit status
 */
int main(int argc, char *argv[], char **envp)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	int status = 0, command_count = 0;
	char *shell_name = argv[0];
	int last_status = 0;

	(void)argc;

	while (1)
	{
		command_count++;
		nread = read_input(&line, &len);

		if (nread == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(last_status);
		}

		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0')
		{
			command_count--;
			continue;
		}

		status = process_input(line, envp, shell_name, command_count, &last_status);
		if (status == -1)
		{
			free(line);
			exit(last_status);
		}
	}
	free(line);
	return (last_status);
}

/**
 * print_error - Print error message
 * @command: Command that failed
 * @shell_name: Name of the shell program
 * @count: Command count
 */
void print_error(char *command, char *shell_name, int count)
{
	fprintf(stderr, "%s: %d: %s: not found\n", shell_name, count, command);
}
