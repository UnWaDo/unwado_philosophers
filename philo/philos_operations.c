/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:41:11 by lalex             #+#    #+#             */
/*   Updated: 2022/07/01 22:41:12 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	destroy_philosopher(t_philo *philo)
{
	pthread_mutex_destroy(&(philo->m_last_eat_time));
	pthread_mutex_destroy(&(philo->m_eat_number));
}

int	create_philosophers(t_philos *philos)
{
	unsigned int	i;

	philos->philos = malloc(
			sizeof(*(philos->philos)) * philos->options[PHILOS_NUMBER]
			);
	if (philos->philos == NULL)
		return (1);
	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		if (create_philosopher(philos, i))
		{
			while (i--)
				destroy_philosopher(philos->philos + i);
			free(philos->philos);
			return (1);
		}
		i++;
	}
	return (0);
}

int	prepare_philosophers(t_philos *philos)
{
	if (create_mutexes(philos))
		return (1);
	if (create_philosophers(philos))
		return (1);
	philos->is_end = 0;
	return (0);
}
