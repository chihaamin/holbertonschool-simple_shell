#include "hsh.h"

/**
 * tokenize_input - Tokenize input string into arguments
 * @input: Input string to tokenize
 * Return: Array of arguments
 */
char **tokenize_input(char *input)
{
	int bufsize = 64, i = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	token = strtok(input, " \t\r\n\a");
	while (token != NULL)
	{
		tokens[i] = token;
		i++;

		if (i >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[i] = NULL;

	return (tokens);
}

/**
 * find_command_path - Find the full path of a command
 * @command: Command to find
 * Return: Full path if found, NULL otherwise
 */
char *find_command_path(char *command)
{
	char *path, *path_copy, *dir, *full_path;
	struct stat st;

	if (strchr(command, '/') != NULL)
		return (stat(command, &st) == 0 && st.st_mode & S_IXUSR)
				   ? strdup(command)
				   : NULL;

	path = getenv("PATH");
	if (!path)
		return (NULL);

	path_copy = strdup(path);
	dir = strtok(path_copy, ":");

	while (dir)
	{
		full_path = malloc(strlen(dir) + strlen(command) + 2);
		if (full_path)
		{
			sprintf(full_path, "%s/%s", dir, command);
			if (stat(full_path, &st) == 0 && st.st_mode & S_IXUSR)
			{
				free(path_copy);
				return (full_path);
			}
			free(full_path);
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * handle_builtins - Handle built-in commands
 * @args: Command and arguments
 * Return: 1 if built-in handled, 0 otherwise
 */
int handle_builtins(char **args)
{
	if (strcmp(args[0], "exit") == 0)
	{
		exit(EXIT_SUCCESS);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		char **env = environ;

		while (*env)
		{
			printf("%s\n", *env);
			env++;
		}
		return (1);
	}

	return (0);
}

/**
 * execute_command - Execute a command
 * @args: Command and arguments
 * @cmd_count: Command count for error messages
 * Return: Exit status
 */
int execute_command(char **args, int cmd_count)
{
	pid_t pid;
	int status;
	char *full_path;

	full_path = find_command_path(args[0]);
	if (!full_path)
	{
		print_error(args[0], cmd_count);
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
	fprintf(stderr, "hsh: %d: %s: not found\n", count, command);
}
