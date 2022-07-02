/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:44 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 14:23:37 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(int argc, char *argv[], unsigned int options[PARAMS_NUMBER])
{
	int				i;

	if (argc < PARAMS_NUMBER || argc > PARAMS_NUMBER + 1)
	{
		printf("Error: invalid number of arguments\n");
		printf(HELP_USAGE, argv[0]);
		return (1);
	}
	i = 1;
	while (i < argc && is_num(argv[i]))
	{
		options[i - 1] = to_num(argv[i]);
		if (options[i - 1] == 0)
			break ;
		i++;
	}
	if (i < argc)
	{
		printf("Error: arguments are invalid or not numeric\n");
		printf(HELP_USAGE, argv[0]);
		return (1);
	}
	if (argc - 1 < PARAMS_NUMBER)
		options[PARAMS_NUMBER - 1] = 0;
	return (0);
}

void	clean_philos(t_philos *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos->options[PHILOS_NUMBER] && waitpid(-1, NULL, 0) > 0)
		i++;
	close_semaphore(philos->s_forks, SEM_FORKS);
	close_semaphore(philos->s_is_start, SEM_START);
	close_semaphore(philos->s_is_end, SEM_END);
	close_semaphore(philos->s_output, SEM_OUTPUT);
	close_semaphore(philos->s_full, SEM_FULL);
}

void	ft_mssleep(unsigned int duration)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (get_ms_timestamp(start) < duration)
		usleep(duration * 10);
}
