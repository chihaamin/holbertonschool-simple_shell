#include "hsh.h"

/**
 * main - Shell, interactive or non interactive
 * @argc: argument count
 * @argv: argument vector
 * @envp: environmental variable
 * Return: 1 if command fails
 */
int main(int argc, char *argv[], char **envp)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char *argv[100];
	pid_t pid;
	int status;
	char *token;
	int i;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "$ ", 2);

		nread = getline(&line, &len, stdin);
		if (nread == -1)
		{
			free(line);
			break;
		}

		/* Remove newline */
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0')
			continue;

		/* Tokenize input into argv[] */
		i = 0;
		token = strtok(line, " \t\n");
		while (token != NULL && i < 99)
		{
			argv[i++] = token;
			token = strtok(NULL, " \t\n");
		}
		argv[i] = NULL;

		if (argv[0] == NULL)
			continue;

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			continue;
		}
		if (pid == 0)
		{
			/* Child */
			if (execve(argv[0], argv, environ) == -1)
			{
				perror("Error");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			/* Parent */
			waitpid(pid, &status, 0);
		}
	}
	return (0);
}
