#include "functions.h"

/**
 * get_environ - returns a copy of the environment variables as a string array
 * @info: parameter struct
 *
 * Return: copy of the environment variables
 */
char **get_environ(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		if (info->environ)
			free_string_array(info->environ);
		info->environ = list_to_strings(info->env);
		info->env_changed = 0;
	}

	return info->environ;
}

/**
 * _unsetenv - removes an environment variable
 * @info: parameter struct
 * @var: the environment variable to remove
 *
 * Return: 1 if variable deleted, 0 otherwise
 */
int _unsetenv(info_t *info, const char *var)
{
	list_t *node = info->env;
	size_t i = 0;

	if (!node || !var)
		return 0;

	while (node)
	{
		if (_starts_with(node->str, var) && node->str[_strlen(var)] == '=')
		{
			info->env_changed = delete_node_at_index(&(info->env), i);
			node = info->env;
			i = 0;
			continue;
		}
		node = node->next;
		i++;
	}

	return info->env_changed;
}

/**
 * _setenv - initializes a new environment variable or modifies an existing one
 * @info: parameter struct
 * @var: the environment variable to set or modify
 * @value: the value to assign to the environment variable
 *
 * Return: 0 on success, 1 on failure
 */
int _setenv(info_t *info, const char *var, const char *value)
{
	char *env_var = NULL;
	list_t *node = info->env;
	size_t var_len, val_len;

	if (!var || !value)
		return 1;

	var_len = _strlen(var);
	val_len = _strlen(value);

	env_var = malloc(var_len + val_len + 2);
	if (!env_var)
		return 1;

	_strcpy(env_var, var);
	_strcat(env_var, "=");
	_strcat(env_var, value);

	while (node)
	{
		if (_starts_with(node->str, var) && node->str[var_len] == '=')
		{
			free(node->str);
			node->str = env_var;
			info->env_changed = 1;
			return 0;
		}
		node = node->next;
	}

	add_node_end(&(info->env), env_var, 0);
	free(env_var);
	info->env_changed = 1;
	return 0;
}
