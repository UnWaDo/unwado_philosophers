#include "philo.h"

static void	destroy_forks(t_philos *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		pthread_mutex_destroy(philos->m_forks + i);
		i++;
	}
}

static int	create_forks(t_philos *philos)
{
	unsigned int	i;

	philos->m_forks = malloc(sizeof(*(philos->m_forks))
			* philos->options[PHILOS_NUMBER]);
	if (philos->m_forks == NULL)
		return (1);
	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		if (pthread_mutex_init(philos->m_forks + i, NULL))
		{
			while (i--)
				pthread_mutex_destroy(philos->m_forks + i);
			return (1);
		}
		i++;
	}
	return (0);
}

int	create_mutexes(t_philos *philos)
{
	if (create_forks(philos))
		return (1);
	if (pthread_mutex_init(&(philos->m_output), NULL))
	{
		destroy_forks(philos);
		return (1);
	}
	if (pthread_mutex_init(&(philos->m_is_start), NULL))
	{
		pthread_mutex_destroy(&(philos->m_output));
		destroy_forks(philos);
		return (1);
	}
	if (pthread_mutex_init(&(philos->m_is_end), NULL))
	{
		pthread_mutex_destroy(&(philos->m_output));
		pthread_mutex_destroy(&(philos->m_is_start));
		destroy_forks(philos);
		return (1);
	}
	return (0);
}

int	create_philosopher(t_philos *philos, int i)
{
	t_philo	*philo;

	philo = philos->philos + i;
	if (pthread_mutex_init(&(philo->m_last_eat_time), NULL))
		return (1);
	if (pthread_mutex_init(&(philo->m_eat_number), NULL))
	{
		pthread_mutex_destroy(&(philo->m_last_eat_time));
		return (1);
	}
	philo->id = i + 1;
	philo->options = philos->options;
	philo->start_time = &(philos->start_time);
	philo->last_eat_time = 0;
	philo->eat_number = 0;
	philo->is_end = &(philos->is_end);
	philo->m_is_end = &(philos->m_is_end);
	philo->m_left_fork = philos->m_forks + i;
	philo->m_right_fork = philos->m_forks
		+ ((i + 1) % philos->options[PHILOS_NUMBER]);
	if (philo->m_right_fork == philo->m_left_fork)
		philo->m_right_fork = NULL;
	philo->m_output = &(philos->m_output);
	philo->m_is_start = &(philos->m_is_start);
	return (0);
}
