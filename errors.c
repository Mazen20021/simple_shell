#include "functions.h"

/**
 * _eputs - prints a string to stderr
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _eputs(const char *str)
{
	size_t i = 0;
	size_t len = strlen(str);

	while (i < len)
		i += write(STDERR_FILENO, str + i, len - i);
}

/**
 * _eputchar - writes a character to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int _eputchar(char c)
{
	static char buf[WRITE_BUF_SIZE];
	static size_t i = 0;

	if (i >= WRITE_BUF_SIZE - 1)
	{
		write(STDERR_FILENO, buf, i);
		i = 0;
	}

	buf[i++] = c;
	if (c == BUF_FLUSH)
	{
		write(STDERR_FILENO, buf, i - 1);
		i = 0;
	}

	return (1);
}

/**
 * _putfd - writes a character to the given file descriptor
 * @c: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 *         On error, -1 is returned, and errno is set appropriately.
 */
int _putfd(char c, int fd)
{
	static char buf[WRITE_BUF_SIZE];
	static size_t i = 0;

	if (i >= WRITE_BUF_SIZE - 1)
	{
		write(fd, buf, i);
		i = 0;
	}

	buf[i++] = c;
	if (c == BUF_FLUSH)
	{
		write(fd, buf, i - 1);
		i = 0;
	}

	return (1);
}

/**
 * _putsfd - prints a string to the given file descriptor
 * @str: The string to be printed
 * @fd: The file descriptor to write to
 *
 * Return: The number of characters written
 */
int _putsfd(const char *str, int fd)
{
	size_t i = 0;
	size_t len = strlen(str);

	while (i < len)
		i += _putfd(str[i], fd);

	return (i);
}
