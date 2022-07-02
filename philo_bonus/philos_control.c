/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:41:09 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 14:23:48 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

uint	get_ms_timestamp(struct timeval start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start.tv_sec) * 1000
		+ (now.tv_usec - start.tv_usec) / 1000);
}

static void	finish_simulation(t_philo *philo)
{
	int	verb;

	verb = 1;
	pthread_mutex_lock(&(philo->m_is_end));
	sem_post(philo->philos->s_is_end);
	if (philo->is_end == 0)
		philo->is_end = 1;
	else
		verb = 0;
	pthread_mutex_unlock(&(philo->m_is_end));
	if (verb)
	{
		sem_wait(philo->philos->s_output);
		printf("%d %d %s\n",
			get_ms_timestamp(philo->philos->start_time), philo->id, "died");
		sem_post(philo->philos->s_output);
	}
}

void	*philo_control(void *philo_raw)
{
	unsigned int	now;
	unsigned int	next_check;
	t_philo			*philo;
	int				ret;

	philo = (t_philo *) philo_raw;
	next_check = philo->philos->options[DIE_TIME];
	while (1)
	{
		now = get_ms_timestamp(philo->philos->start_time);
		if (next_check > now)
			ft_mssleep(next_check - now);
		pthread_mutex_lock(&(philo->m_last_eat_time));
		now = get_ms_timestamp(philo->philos->start_time);
		if (now >= philo->last_eat_time + philo->philos->options[DIE_TIME])
		{
			finish_simulation(philo);
			ret = 1;
		}
		next_check = philo->last_eat_time + philo->philos->options[DIE_TIME];
		pthread_mutex_unlock(&(philo->m_last_eat_time));
		if (ret)
			return (0);
	}
	return (0);
}
