/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_other.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:41:07 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 13:49:26 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_is_end(t_philo *philo)
{
	int	end;

	end = 0;
	pthread_mutex_lock(&(philo->m_is_end));
		end = philo->is_end;
	pthread_mutex_unlock(&(philo->m_is_end));
	return (end);
}

int	philo_write(t_philo *philo, char *str)
{
	int	ret;

	ret = 0;
	sem_wait(philo->philos->s_output);
	if (!philo_is_end(philo))
	{
		printf("%d %d %s\n",
			get_ms_timestamp(philo->philos->start_time), philo->id, str);
	}
	else
		ret = 1;
	sem_post(philo->philos->s_output);
	return (ret);
}

int	philo_sleep(t_philo *philo)
{
	if (philo_write(philo, "is sleeping"))
		return (1);
	ft_mssleep(philo->philos->options[SLEEP_TIME]);
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
	sem_wait(philo->philos->s_is_start);
	sem_post(philo->philos->s_is_start);
	gettimeofday(&(philo->philos->start_time), NULL);
	philo_think(philo);
	if (philo->id % 2 == 0)
		ft_mssleep(philo->philos->options[EAT_TIME]);
	else if (philo->id == philo->philos->options[PHILOS_NUMBER])
		ft_mssleep(philo->philos->options[EAT_TIME] * 2);
	if (philo->philos->options[PHILOS_NUMBER] == 1)
		return (0);
	while (!philo_eat(philo) && !philo_sleep(philo)
		&& !philo_think(philo));
	return (0);
}
