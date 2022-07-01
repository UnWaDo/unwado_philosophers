/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_control.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:41:09 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 00:47:41 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	get_ms_timestamp(struct timeval start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start.tv_sec) * 1000
		+ (now.tv_usec - start.tv_usec) / 1000);
}

static int	check_death(t_philo *philo, unsigned int *min_death_time)
{
	unsigned int	now;
	unsigned int	ret;

	ret = 0;
	pthread_mutex_lock(&(philo->m_last_eat_time));
	now = get_ms_timestamp(*(philo->start_time));
	if (now - philo->last_eat_time > philo->options[DIE_TIME])
	{
		pthread_mutex_lock(philo->m_is_end);
			philo->is_end[0] = 1;
		pthread_mutex_unlock(philo->m_is_end);
		ret = 1;
	}
	else if (philo->last_eat_time + philo->options[DIE_TIME] < *min_death_time)
		*min_death_time = philo->last_eat_time + philo->options[DIE_TIME];
	pthread_mutex_unlock(&(philo->m_last_eat_time));
	if (ret)
	{
		pthread_mutex_lock(philo->m_output);
		printf("%d %d died\n",
			get_ms_timestamp(*(philo->start_time)), philo->id);
		pthread_mutex_unlock(philo->m_output);
	}
	return (ret);
}

static int	check_ate_enough(t_philos *philos)
{
	unsigned int	i;
	int				all_ate;

	if (philos->options[EAT_NUMBER] <= 0)
		return (0);
	all_ate = 1;
	i = 0;
	while (i < philos->options[PHILOS_NUMBER] && all_ate)
	{
		pthread_mutex_lock(&(philos->philos[i].m_eat_number));
		if (philos->philos[i].eat_number < philos->options[EAT_NUMBER])
			all_ate = 0;
		pthread_mutex_unlock(&(philos->philos[i].m_eat_number));
		i++;
	}
	if (!all_ate)
		return (0);
	pthread_mutex_lock(&(philos->m_is_end));
		philos->is_end = 1;
	pthread_mutex_unlock(&(philos->m_is_end));
	return (1);
}

void	control_simulation(t_philos *philos)
{
	unsigned int	i;
	unsigned int	next_check;
	unsigned int	now;

	if (philos->options[DIE_TIME] > philos->options[EAT_TIME])
		next_check = philos->options[EAT_TIME];
	else
		next_check = philos->options[DIE_TIME];
	while (!philos->is_end)
	{
		now = get_ms_timestamp(philos->start_time);
		if (next_check > now + 1)
			usleep((next_check - now - 1) * 1000);
		i = 0;
		while (i < philos->options[PHILOS_NUMBER]
			&& !check_death(philos->philos + i, &next_check))
			i++;
		if (i < philos->options[PHILOS_NUMBER] || check_ate_enough(philos))
			break ;
	}
}
