int	is_num(char *str)
{
	while (*str && (*str >= '0' && *str <= '9'))
		str++;
	return (*str == '\0');
}

unsigned int	to_num(char *str)
{
	long	val;

	val = 0;
	while (*str)
	{
		if (val > val * 10 + ((*str) - '0'))
			return (-1);
		val = val * 10 + ((*str) - '0');
		if (val > __INT_MAX__)
			return (0);
		str++;
	}
	return ((unsigned int) val);
}
