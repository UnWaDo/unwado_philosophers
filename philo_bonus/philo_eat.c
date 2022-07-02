/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_eat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:40:03 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 13:10:33 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_try_taking_fork(t_philo *philo)
{
	sem_wait(philo->philos->s_forks);
	if (!philo_write(philo, "has taken a fork"))
		return (0);
	sem_post(philo->philos->s_forks);
	return (1);
}

static int	philo_await(t_philo *philo)
{
	uint	x;

	if (philo->philos->options[PHILOS_NUMBER] % 2 == 0)
		return (0);
	x = (philo->id - 1
			- 1 * (philo->id == philo->philos->options[PHILOS_NUMBER])) / 2 + 1;
	if (philo->eat_number >= x && (philo->eat_number - x) % 2 == 0)
		ft_mssleep(philo->philos->options[EAT_TIME]);
	return (0);
}

static int	philo_take_forks(t_philo *philo)
{
	philo_await(philo);
	if (philo_try_taking_fork(philo))
		return (1);
	if (philo_try_taking_fork(philo))
	{
		sem_post(philo->philos->s_forks);
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
		philo->last_eat_time = get_ms_timestamp(philo->philos->start_time);
	pthread_mutex_unlock(&(philo->m_last_eat_time));
	ret = 0;
	if (!philo_write(philo, "is eating"))
		ft_mssleep(philo->philos->options[EAT_TIME]);
	else
		ret = 1;
	sem_post(philo->philos->s_forks);
	sem_post(philo->philos->s_forks);
	philo->eat_number++;
	if (philo->eat_number == philo->philos->options[EAT_NUMBER])
		sem_post(philo->philos->s_full);
	return (ret);
}
