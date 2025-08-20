#include "hsh.h"

#include "hsh.h"

int command_count = 0;
char *shell_name;

/**
 * execute_command - Execute a command
 * @args: Command and arguments
 * Return: Exit status
 */
int execute_command(char **args)
{
	pid_t pid;
	int status;
	char *full_path;

	full_path = find_command_path(args[0]);
	if (!full_path)
	{
		print_error(args[0], command_count);
		return (127);
	}

	pid = fork();
	if (pid == 0)
	{
		/* Child process */
		if (execve(full_path, args, environ) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	else if (pid < 0)
	{
		/* Fork failed */
		perror("fork");
		free(full_path);
		return (1);
	}
	else
	{
		/* Parent process */
		waitpid(pid, &status, 0);
		free(full_path);
	}

	return (WEXITSTATUS(status));
}

/**
 * print_error - Print error message
 * @command: Command that failed
 * @count: Command count
 */
void print_error(char *command, int count)
{
	fprintf(stderr, "%s: %d: %s: not found\n", shell_name, count, command);
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
	char **args;
	int status = 0;

	(void)argc;
	(void)envp;

	shell_name = argv[0];
	command_count = 0;

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
		if (handle_builtins(args))
		{
			free(args);
			continue;
		}

		/* Execute external command */
		status = execute_command(args);
		free(args);
	}

	free(line);
	return (status);
}
