#include "hsh.h"

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
	char **args;
	int status = 0, command_count = 0;
	char *shell_name = argv[0];

	(void)argc;

	while (1)
	{
		command_count++;

		/* Display prompt in interactive mode */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);

		/* Read input */
		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			exit(status);
		}

		/* Remove newline */
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Skip empty lines */
		if (line[0] == '\0')
		{
			command_count--;
			continue;
		}

		/* Tokenize input */
		args = tokenize_input(line);
		if (args[0] == NULL)
		{
			free(args);
			command_count--;
			continue;
		}

		/* Handle built-in commands */
		if (handle_builtins(args, envp))
		{
			free(args);
			continue;
		}

		/* Execute external command */
		status = execute_command(args, envp, shell_name, command_count);
		free(args);
	}

	free(line);
	return (status);
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