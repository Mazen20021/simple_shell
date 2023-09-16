#include "functions.h"

/**
 * display_history - Displays the history list, one command per line,
 *                   preceded with line numbers starting at 0.
 * @info: Structure containing the history list.
 */
void display_history(const info_t *info)
{
  int line_number = 0;
  list_t *node = info->history;

  while (node != NULL)
  {
    printf("%d %s\n", line_number, node->str);
    node = node->next;
    line_number++;
  }
}

/**
 * remove_alias - Removes an alias from the alias list.
 * @info: Structure containing the alias list.
 * @alias: The alias to remove.
 * Return: 0 on success, 1 on error.
 */
int remove_alias(info_t *info, const char *alias)
{
  return delete_node(&(info->alias), alias);
}

/**
 * add_alias - Adds an alias to the alias list.
 * @info: Structure containing the alias list.
 * @alias: The alias to add.
 * Return: 0 on success, 1 on error.
 */
int add_alias(info_t *info, const char *alias)
{
  // Remove existing alias with the same name (if any)
  remove_alias(info, alias);

  return add_node_end(&(info->alias), alias, 0) == NULL ? 1 : 0;
}

/**
 * print_alias - Prints an alias string.
 * @alias: The alias string to print.
 */
void print_alias(const char *alias)
{
  printf("%s\n", alias);
}

/**
 * display_aliases - Displays the alias list.
 * @info: Structure containing the alias list.
 */
void display_aliases(const info_t *info)
{
  list_t *node = info->alias;

  while (node != NULL)
  {
    print_alias(node->str);
    node = node->next;
  }
}

/**
 * handle_alias_command - Handles the 'alias' command.
 * @info: Structure containing the alias list and command arguments.
 * Return: Always 0.
 */
int handle_alias_command(info_t *info)
{
  int i;
  const char *arg;

  // If no arguments provided, display all aliases
  if (info->argc == 1)
  {
    display_aliases(info);
    return 0;
  }

  // Process each argument
  for (i = 1; i < info->argc; i++)
  {
    arg = info->argv[i];

    // Check if the argument contains '='
    const char *equals = strchr(arg, '=');
    if (equals != NULL)
    {
      // If '=' found, set an alias
      add_alias(info, arg);
    }
    else
    {
      // If no '=', print the alias if it exists
      list_t *alias_node = node_starts_with(info->alias, arg, '=');
      if (alias_node != NULL)
        print_alias(alias_node->str);
    }
  }

  return 0;
}
