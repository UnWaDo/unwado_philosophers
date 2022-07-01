/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:51 by lalex             #+#    #+#             */
/*   Updated: 2022/07/01 22:39:52 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
