/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:44 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 13:10:05 by lalex            ###   ########.fr       */
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

static void	finish_simulation(t_philo *philo)
{
	pthread_mutex_lock(&(philo->m_is_end));
	philo->is_end = 1;
	pthread_mutex_unlock(&(philo->m_is_end));
	sem_post(philo->philos->s_is_end);
	sem_wait(philo->philos->s_output);
	printf("%d %d %s\n",
		get_ms_timestamp(philo->philos->start_time), philo->id, "died");
	sem_post(philo->philos->s_output);
}

static void	*philo_control(void *philo_raw)
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
	sem_post(philos->s_is_end);
	pthread_mutex_lock(&(philo.m_is_end));
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
