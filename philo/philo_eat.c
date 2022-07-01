/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:40:03 by lalex             #+#    #+#             */
/*   Updated: 2022/07/01 22:41:05 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_try_taking_fork(t_philo *philo, pthread_mutex_t *fork)
{
	pthread_mutex_lock(fork);
	if (!philo_write(philo, "has taken a fork"))
		return (0);
	pthread_mutex_unlock(fork);
	return (1);
}

static int	philo_await(t_philo *philo)
{
	unsigned int	x;

	if (philo->eat_number == 0 && philo->id % 2 == 0)
		usleep(philo->options[EAT_TIME] * 1000);
	if (philo->options[PHILOS_NUMBER] % 2
		&& philo->options[EAT_TIME] >= philo->options[SLEEP_TIME])
	{
		if (philo->eat_number == 0
			&& philo->id == philo->options[PHILOS_NUMBER])
			usleep(philo->options[EAT_TIME] * 2 * 1000);
		x = (philo->id - 1 - 1 * (philo->id == philo->options[PHILOS_NUMBER]))
			/ 2 + 1;
		if (philo->eat_number >= x && (philo->eat_number - x) % 2 == 0)
			usleep((philo->options[EAT_TIME]) * 1000);
	}
	return (0);
}

int	philo_take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->id % 2)
	{
		first = philo->m_left_fork;
		second = philo->m_right_fork;
	}
	else
	{
		first = philo->m_right_fork;
		second = philo->m_left_fork;
	}
	philo_await(philo);
	if (philo_try_taking_fork(philo, first))
		return (1);
	if (philo_try_taking_fork(philo, second))
	{
		pthread_mutex_unlock(first);
		return (1);
	}
	return (0);
}

int	philo_eat(t_philo *philo)
{
	int	ret;

	if (philo_is_end(philo))
		return (1);
	if (philo_take_forks(philo))
		return (1);
	pthread_mutex_lock(&(philo->m_last_eat_time));
		philo->last_eat_time = get_ms_timestamp(philo->start_time[0]);
	pthread_mutex_unlock(&(philo->m_last_eat_time));
	ret = 0;
	if (!philo_write(philo, "is eating"))
		usleep(philo->options[EAT_TIME] * 1000);
	else
		ret = 1;
	pthread_mutex_unlock(philo->m_right_fork);
	pthread_mutex_unlock(philo->m_left_fork);
	pthread_mutex_lock(&(philo->m_eat_number));
		philo->eat_number++;
	pthread_mutex_unlock(&(philo->m_eat_number));
	return (ret);
}
