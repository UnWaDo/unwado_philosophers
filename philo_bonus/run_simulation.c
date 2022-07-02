/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/02 14:22:42 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 14:24:31 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_philo(t_philo *philo, t_philos *philos, unsigned int id)
{
	philo->philos = philos;
	philo->id = id;
	philo->last_eat_time = 0;
	philo->eat_number = 0;
	philo->is_end = 0;
	if (pthread_mutex_init(&(philo->m_is_end), NULL))
		return (1);
	if (pthread_mutex_init(&(philo->m_last_eat_time), NULL))
	{
		pthread_mutex_destroy(&(philo->m_is_end));
		return (1);
	}
	return (0);
}

static int	philo_init(t_philos *philos, unsigned int id)
{
	pthread_t	philo_thread;
	pthread_t	control_thread;
	t_philo		philo;

	sem_wait(philos->s_is_start);
	sem_post(philos->s_is_start);
	if (create_philo(&philo, philos, id))
	{
		sem_post(philos->s_is_end);
		return (1);
	}
	pthread_create(&philo_thread, NULL, philo_life, &philo);
	pthread_create(&control_thread, NULL, philo_control, &philo);
	sem_wait(philos->s_is_end);
	pthread_mutex_lock(&(philo.m_is_end));
	sem_post(philos->s_is_end);
	philo.is_end = 1;
	pthread_mutex_unlock(&(philo.m_is_end));
	pthread_join(philo_thread, NULL);
	pthread_join(control_thread, NULL);
	pthread_mutex_destroy(&(philo.m_is_end));
	pthread_mutex_destroy(&(philo.m_last_eat_time));
	return (0);
}

static void	clean_philosopher(t_philos *philos)
{
	sem_close(philos->s_forks);
	sem_close(philos->s_is_end);
	sem_close(philos->s_is_start);
	sem_close(philos->s_full);
	sem_close(philos->s_output);
}

int	run_simulation(t_philos *philos)
{
	unsigned int	i;
	pid_t			pid;

	i = 0;
	gettimeofday(&(philos->start_time), NULL);
	while (i < philos->options[PHILOS_NUMBER])
	{
		pid = fork();
		if (pid < 0)
			return (1);
		if (!pid)
		{
			philo_init(philos, i + 1);
			clean_philosopher(philos);
			exit(0);
		}
		i++;
	}
	sem_post(philos->s_is_start);
	return (0);
}
