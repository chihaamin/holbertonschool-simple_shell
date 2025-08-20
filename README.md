# Simple Shell Project

### Description
This is a simple UNIX command line interpreter (shell) implemented in C as part of the Holberton School curriculum. The shell supports basic command execution, built-in commands, and proper error handling.

## Features
- Displays a prompt and waits for user input

- Handles commands with arguments

- Implements PATH resolution for command execution

- Supports built-in commands: exit and env

- Proper error handling with appropriate error messages

- Handles both interactive and non-interactive modes

- Memory leak-free implementation

- Betty-style compliant code

## Built-in Commands
- `exit`: Exits the shell with the status of the last executed command

- `env`: Prints the current environment variables

## Compilation
Compile the shell using the following command:

```bash
gcc -Wall -Werror -Wextra -pedantic *.c -o hsh
```
## Usage

### Interactive Mode

Run the shell and enter commands at the prompt:

```bash
$ ./hsh
$ /bin/ls
hsh main.c utils.c hsh.h
$ exit
```
### Non-Interactive Mode

Pipe commands to the shell:

```bash
$ echo "/bin/ls" | ./hsh
hsh main.c utils.c hsh.h
$ echo "exit" | ./hsh
```
------

### Examples
``` bash
$ ./hsh
$ ls
hsh main.c utils.c hsh.h
$ /bin/ls -l
total 32
-rwxr-xr-x 1 user user 16856 May 15 10:30 hsh
-rw-r--r-- 1 user user  1234 May 15 10:30 main.c
-rw-r--r-- 1 user user  2345 May 15 10:30 utils.c
-rw-r--r-- 1 user user   456 May 15 10:30 hsh.h
$ env
USER=user
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
...
$ exit
``` 
## Error Handling
The shell provides appropriate error messages when commands cannot be found or executed:

```bash
$ ./hsh
$ nonexistent_command
./hsh: 1: nonexistent_command: not found
$ /bin/ls /nonexistent_directory
/bin/ls: cannot access '/nonexistent_directory': No such file or directory
```

## File Structure
- `main.c`: Contains the main shell loop and input processing

- `utils.c`: Contains helper functions for tokenization, PATH resolution, and command execution

- `hsh.h`: Header file with function prototypes and includes

## Allowed Functions
The project uses a limited set of functions as required:

`access`, `chdir`, `close`, `closedir`

`execve`, `exit`, `_exit`, `fflush`

`fork`, `free`, `getcwd`, `getline`

`getpid`, `isatty`, `kill`, `malloc`

`open`, `opendir`, `perror`, `read`

`readdir`, `signal`, `stat`, `lstat`, `fstat`

`strtok`, `wait`, `waitpid`, `wait3`, `wait4`

`write`, and functions from `string.h`

Authors
Amin Chiha <amin.chiha@gmail.com>

License
This project is part of the Holberton School curriculum and is for educational purposes.
