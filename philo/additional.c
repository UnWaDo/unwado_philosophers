/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:44 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 06:19:27 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(int argc, char *argv[], unsigned int options[PARAMS_NUMBER])
{
	int	i;

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

int	run_simulation(t_philos *philos)
{
	unsigned int	i;

	i = 0;
	pthread_mutex_lock(&(philos->m_is_start));
	while (i < philos->options[PHILOS_NUMBER])
	{
		philos->philos[i].thread = 0;
		if (pthread_create(&(philos->philos[i].thread), NULL,
				philo_life, philos->philos + i))
		{
			pthread_mutex_lock(&(philos->m_is_end));
			philos->is_end = 1;
			pthread_mutex_unlock(&(philos->m_is_end));
			while (i--)
				pthread_detach(philos->philos[i].thread);
			pthread_mutex_unlock(&(philos->m_is_start));
			return (1);
		}
		i++;
	}
	gettimeofday(&(philos->start_time), NULL);
	pthread_mutex_unlock(&(philos->m_is_start));
	return (0);
}

void	clean_philos(t_philos *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos->options[PHILOS_NUMBER] && philos->philos[i].thread != 0)
	{
		pthread_join(philos->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		destroy_philosopher(philos->philos + i);
		pthread_mutex_destroy(philos->m_forks + i);
		i++;
	}
	free(philos->philos);
	free(philos->m_forks);
	pthread_mutex_destroy(&(philos->m_is_end));
	pthread_mutex_destroy(&(philos->m_output));
	pthread_mutex_destroy(&(philos->m_is_start));
}

void	ft_mssleep(unsigned int duration)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (get_ms_timestamp(start) < duration)
		usleep(duration * 10);
}
