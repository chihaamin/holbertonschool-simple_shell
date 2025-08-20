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
