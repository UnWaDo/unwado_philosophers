/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_other.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:41:07 by lalex             #+#    #+#             */
/*   Updated: 2022/07/01 22:41:07 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_is_end(t_philo *philo)
{
	int	end;

	end = 0;
	pthread_mutex_lock(philo->m_is_end);
		end = philo->is_end[0];
	pthread_mutex_unlock(philo->m_is_end);
	return (end);
}

int	philo_write(t_philo *philo, char *str)
{
	int	ret;

	ret = 0;
	pthread_mutex_lock(philo->m_output);
	if (!philo_is_end(philo))
		printf("%d %d %s\n",
			get_ms_timestamp(philo->start_time[0]), philo->id, str);
	else
		ret = 1;
	pthread_mutex_unlock(philo->m_output);
	return (ret);
}

int	philo_sleep(t_philo *philo)
{
	if (philo_write(philo, "is sleeping"))
		return (1);
	usleep(philo->options[SLEEP_TIME] * 1000);
	return (0);
}

int	philo_think(t_philo *philo)
{
	if (philo_write(philo, "is thinking"))
		return (1);
	return (0);
}

void	*philo_life(void *philo_raw)
{
	t_philo	*philo;

	philo = (t_philo *) philo_raw;
	pthread_mutex_lock(philo->m_is_start);
	pthread_mutex_unlock(philo->m_is_start);
	if (philo->options[PHILOS_NUMBER] == 1)
		return (0);
	while (!philo_eat(philo) && !philo_sleep(philo)
		&& !philo_think(philo));
	return (0);
}
