#include "functions.h"

/**
 * clear_info - initializes info_t struct
 * @info: struct address
 */
void clear_info(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void set_info(info_t *info, char **av)
{
	int i;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = strtow(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = _strdup(info->arg);
				info->argv[1] = NULL;
			}
		}
		info->argc = 0;
		if (info->argv)
		{
			for (i = 0; info->argv[i]; i++)
				info->argc++;
		}

		replace_alias(info);
		replace_vars(info);
	}
}

/**
 * free_info - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *info, int all)
{
	if (info->argv)
	{
		ffree(info->argv);
		info->argv = NULL;
		info->argc = 0;
	}

	if (all)
	{
		if (!info->cmd_buf)
		{
			free(info->arg);
			info->arg = NULL;
		}

		free_list(&(info->env));
		info->env = NULL;

		free_list(&(info->history));
		info->history = NULL;

		free_list(&(info->alias));
		info->alias = NULL;

		ffree(info->environ);
		info->environ = NULL;

		bfree((void **)&info->cmd_buf);
		info->cmd_buf = NULL;

		if (info->readfd > 2)
		{
			close(info->readfd);
			info->readfd = -1;
		}

		_putchar(BUF_FLUSH);
	}
}
