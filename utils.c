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
 * _getenv - Get environment variable value
 * @name: Variable name
 * @envp: Environment variables array
 * Return: Value of variable or NULL if not found
 */
char *_getenv(char *name, char **envp)
{
	int i = 0;
	size_t len = strlen(name);

	while (envp[i] != NULL)
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

/**
 * find_command_path - Find the full path of a command
 * @command: Command to find
 * @envp: Environment variables array
 * Return: Full path if found, NULL otherwise
 */
char *find_command_path(char *command, char **envp)
{
	char *path, *path_copy, *dir, *full_path;
	struct stat st;

	if (strchr(command, '/') != NULL)
		return ((stat(command, &st) == 0 && st.st_mode & S_IXUSR)
					? strdup(command)
					: NULL);

	path = _getenv("PATH", envp);
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
 * execute_command - Execute a command
 * @args: Command and arguments
 * @envp: Environment variables array
 * @shell_name: Name of the shell program
 * @count: Command count for error messages
 * Return: Exit status
 */
int execute_command(char **args, char **envp, char *shell_name, int count)
{
	pid_t pid;
	int status;
	char *full_path;

	full_path = find_command_path(args[0], envp);
	if (!full_path)
	{
		print_error(args[0], shell_name, count);
		return (127);
	}

	pid = fork();
	if (pid == 0)
	{
		if (execve(full_path, args, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	else if (pid < 0)
	{
		perror("fork");
		free(full_path);
		return (1);
	}
	else
	{
		waitpid(pid, &status, 0);
		free(full_path);
	}

	return (0);
}

/**
 * handle_builtins - Handle built-in commands
 * @args: Command and arguments
 * @envp: Environment variables array
 * Return: 2 for exit, 1 for env, 0 for not built-in
 */
int handle_builtins(char **args, char **envp)
{
	if (strcmp(args[0], "exit") == 0)
	{
		return (2);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		int i = 0;

		while (envp[i] != NULL)
		{
			printf("%s\n", envp[i]);
			i++;
		}
		return (1);
	}

	return (0);
}
