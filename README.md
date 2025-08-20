Simple Shell
============

simple_shell is a minimal UNIX command line interpreter implemented in C.

Features
- Displays a prompt and reads a command line.
- Supports single-word commands (no arguments in this version).
- Executes commands via execve and passes the environment to child processes.
- Handles end-of-file (Ctrl+D) to exit.

Usage

Compile:

```sh
gcc -Wall -Werror -Wextra -pedantic main.c -o simple_shell
```

Run:

```sh
./simple_shell
$ /bin/ls
```

Notes
- This project implements the basic behavior requested by the Holberton "simple_shell" task (0.1). It does not implement PATH searching, built-ins, piping, redirections, or command-line arguments.

Files
- `main.c` - shell entrypoint and main loop
- `hsh.h` - project header
- `man_1_simple_shell` - man page source
- `AUTHORS` - contributors list

License
-------
Public domain / educational purposes.
-------

#### Authors

    Amin Chiha <amin.chiha@gmail.com>