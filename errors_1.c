#include "functions.h"

/**
 * _erratoi - converts a string to an integer
 * @s: the string to be converted
 *
 * Return: 0 if no numbers in string, converted number otherwise
 *         -1 on error
 */
int _erratoi(const char *s)
{
	int i = 0;
	unsigned long result = 0;

	if (*s == '+')
		s++; /* Skip leading plus sign */

	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}

	return ((int)result);
}

/**
 * print_error - prints an error message
 * @info: the parameter and return info struct
 * @estr: string containing specified error type
 */
void print_error(const info_t *info, const char *estr)
{
	_eputs(info->fname);
	_eputs(": ");
	print_d(info->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(info->argv[0]);
	_eputs(": ");
	_eputs(estr);
}

/**
 * print_d - prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the file descriptor to write to
 *
 * Return: number of characters printed
 */
int print_d(int input, int fd)
{
	int (*putchar_func)(char) = _putchar;
	int count = 0;
	unsigned int abs_val, current;
	char sign = 0;

	if (fd == STDERR_FILENO)
		putchar_func = _eputchar;

	if (input < 0)
	{
		abs_val = (unsigned int)-input;
		sign = '-';
	}
	else
		abs_val = (unsigned int)input;

	if (sign)
	{
		putchar_func(sign);
		count++;
	}

	current = abs_val;
	do
	{
		putchar_func('0' + current / 10);
		count++;
		current %= 10;
	} while (current != 0);

	return (count);
}

/**
 * convert_number - converts a number to a string
 * @num: number
 * @base: base
 *
 * Return: string representation of the number
 */
char *convert_number(long int num, int base)
{
	static char buffer[50];
	const char *array = "0123456789ABCDEF";
	char *ptr;
	unsigned long n = num;
	char sign = 0;

	if (num < 0)
	{
		n = -num;
		sign = '-';
	}

	ptr = &buffer[49];
	*ptr = '\0';

	do
	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;

	return (ptr);
}

/**
 * remove_comments - replaces the first instance of '#' with '\0'
 * @buf: address of the string to modify
 */
void remove_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
	{
		if (buf[i] == '#' && (i == 0 || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
	}
}
