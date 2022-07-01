#include "philo.h"

unsigned int	get_ms_timestamp(struct timeval start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start.tv_sec) * 1000
		+ (now.tv_usec - start.tv_usec) / 1000);
}

int	check_death(t_philo *philo)
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

int	check_ate_enough(t_philos *philos)
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

	while (!philos->is_end)
	{
		i = 0;
		while (i < philos->options[PHILOS_NUMBER]
			&& !check_death(philos->philos + i))
			i++;
		if (i < philos->options[PHILOS_NUMBER] || check_ate_enough(philos))
			break ;
	}
}
