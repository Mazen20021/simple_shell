#include "functions.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
    ssize_t r = 0;
    int builtin_ret = 0;

    while (r != -1 && builtin_ret != -2)
    {
        clear_info(info);
        if (interactive(info))
            _puts("$ ");
        _eputchar(BUF_FLUSH);
        r = get_input(info);
        if (r != -1)
        {
            set_info(info, av);
            builtin_ret = find_builtin(info);
            if (builtin_ret == -1)
                find_cmd(info);
        }
        else if (interactive(info))
            _putchar('\n');
        free_info(info, 0);
    }
    write_history(info);
    free_info(info, 1);
    if (!interactive(info) && info->status)
        exit(info->status);
    if (builtin_ret == -2)
    {
        if (info->err_num == -1)
            exit(info->status);
        exit(info->err_num);
    }
    return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if builtin not found,
 *          0 if builtin executed successfully,
 *          1 if builtin found but not successful,
 *          -2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
    int i, built_in_ret = -1;
    builtin_table builtintbl[] = {
        {"exit", _myexit},
        {"env", _myenv},
        {"help", _myhelp},
        {"history", _myhistory},
        {"setenv", _mysetenv},
        {"unsetenv", _myunsetenv},
        {"cd", _mycd},
        {"alias", _myalias},
        {NULL, NULL}
    };

    for (i = 0; builtintbl[i].type; i++)
    {
        if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
        {
            info->line_count++;
            built_in_ret = builtintbl[i].func(info);
            break;
        }
    }
    return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_t *info)
{
    char *path = NULL;
    int i, k;

    info->path = info->argv[0];
    if (info->linecount_flag == 1)
    {
        info->line_count++;
        info->linecount_flag = 0;
    }
    for (i = 0, k = 0; info->arg[i]; i++)
    {
        if (!is_delim(info->arg[i], " \t\n"))
            k++;
    }
    if (!k)
        return;

    path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
    if (path)
    {
        info->path = path;
        fork_cmd(info);
    }
    else
    {
        if ((interactive(info) || _getenv(info, "PATH=") || info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
            fork_cmd(info);
        else if (*(info->arg) != '\n')
        {
            info->status = 127;
            print_error(info, "not found\n");
        }
    }
}

/**
 * fork_cmd - forks an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
    pid_t child_pid;

    child_pid = fork();
    if (child_pid == -1)
    {
        perror("Error:");
        return;
    }
    if (child_pid == 0)
    {
        if (execve(info->path, info->argv, get_environ(info)) == -1)
        {
            free_info(info, 1);
            if (errno == EACCES)
                exit(126);
            exit(1);
        }
    }
    else
    {
        wait(&(info->status));
        if (WIFEXITED(info->status))
        {
            info->status = WEXITSTATUS(info->status);
            if (info->status == The code you provided seems to be a part of a shell program. It defines several functions that are used in the main shell loop. Here is a breakdown of the functions:

1. `hsh`: This is the main shell loop function. It takes an `info_t` struct and an argument vector `av` as parameters. It contains a while loop that continues until the return value of `get_input` is `-1` or the return value of `find_builtin` is `-2`. Within the loop, it clears the `info` struct, prompts for user input if it's in interactive mode, reads user input using `get_input`, sets the `info` struct, and then calls `find_builtin` or `find_cmd` functions based on whether the input is a built-in command or an external command. After the loop, it writes the command history, frees the `info` struct, and exits the program if necessary.

2. `find_builtin`: This function takes the `info` struct as a parameter and searches for a built-in command in the `info->argv[0]` using a table of built-in commands and their corresponding function pointers (`builtin_table`). If a match is found, it increments the `line_count` in `info` and calls the corresponding built-in function. It returns the return value of the built-in function or `-1` if the built-in command is not found.

3. `find_cmd`: This function is called when the input is not a built-in command. It takes the `info` struct as a parameter and searches for the command in the `PATH` environment variable using the `find_path` function. If a valid path is found, it sets the `info->path` and calls the `fork_cmd` function. Otherwise, it checks if the input is a valid command by calling the `is_cmd` function and `fork_cmd` if it is. If the command is not found, it sets the `info->status` to `127` and prints an error message.

4. `fork_cmd`: This function is responsible for forking a child process and executing the command. It takes the `info` struct as a parameter. It calls `fork` to create a new process. In the child process, it calls `execve` to execute the command specified by `info->path` with the arguments `info->argv` and the environment variables returned by `get_environ`. If `execve` fails, it frees the `info` struct and exits the child process with an appropriate exit code. In the parent process, it waits for the child process to complete and updates the `info->status` with the exit status of the child process.

Overall, these functions seem to be part of a shell program that handles both built-in commands and external commands by searching for the command in the `PATH` and executing it in a child process.
