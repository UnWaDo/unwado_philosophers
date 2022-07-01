#include "philo.h"

static unsigned int	*check_args_num(int argc, char *argv[])
{
	unsigned int	*options;

	if (argc < PARAMS_NUMBER || argc > PARAMS_NUMBER + 1)
	{
		printf("Error: invalid number of arguments\n");
		printf(HELP_USAGE, argv[0]);
		return (NULL);
	}
	options = malloc(sizeof(*options) * PARAMS_NUMBER);
	if (!options)
	{
		printf("Error: memory allocation failed\n");
		return (NULL);
	}
	return (options);
}

unsigned int	*parse_args(int argc, char *argv[])
{
	unsigned int	*options;
	int				i;

	options = check_args_num(argc, argv);
	if (!options)
		return (NULL);
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
		free(options);
		return (NULL);
	}
	if (argc - 1 < PARAMS_NUMBER)
		options[PARAMS_NUMBER - 1] = 0;
	return (options);
}

int	run_simulation(t_philos *philos)
{
	unsigned int	i;

	i = 0;
	pthread_mutex_lock(&(philos->m_is_start));
	while (i < philos->options[PHILOS_NUMBER])
	{
		philos->philos[i].thread = 0;
		if (pthread_create(&(philos->philos[i].thread), NULL,
				philo_life, philos->philos + i))
		{
			pthread_mutex_lock(&(philos->m_is_end));
			philos->is_end = 1;
			pthread_mutex_unlock(&(philos->m_is_end));
			while (i--)
				pthread_detach(philos->philos[i].thread);
			pthread_mutex_unlock(&(philos->m_is_start));
			return (1);
		}
		i++;
	}
	gettimeofday(&(philos->start_time), NULL);
	pthread_mutex_unlock(&(philos->m_is_start));
	return (0);
}

void	clean_philos(t_philos *philos)
{
	unsigned int	i;

	i = 0;
	while (i < philos->options[PHILOS_NUMBER] && philos->philos[i].thread != 0)
	{
		pthread_join(philos->philos[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		destroy_philosopher(philos->philos + i);
		pthread_mutex_destroy(philos->m_forks + i);
		i++;
	}
	free(philos->philos);
	free(philos->m_forks);
	free(philos->options);
	pthread_mutex_destroy(&(philos->m_is_end));
	pthread_mutex_destroy(&(philos->m_output));
	pthread_mutex_destroy(&(philos->m_is_start));
}
