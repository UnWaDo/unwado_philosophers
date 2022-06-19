#include <printf.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

# define	HELP_USAGE	"Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [times_to_eat]\n"

int	get_ms_timestamp(struct timeval start);

enum	PARAMS
{
	PHILOS_NUMBER,
	DIE_TIME,
	EAT_TIME,
	SLEEP_TIME,
	EAT_NUMBER,
	PARAMS_NUMBER
};

typedef struct s_philo {
	pthread_t		thread;
	int				id;
	int				*options;
	struct timeval	*start_time;
	int				last_eat_time;
	pthread_mutex_t	m_last_eat_time;
	int				eat_number;
	pthread_mutex_t	m_eat_number;
	int				*is_end;
	pthread_mutex_t	*m_is_end;
	pthread_mutex_t	*m_left_fork;
	pthread_mutex_t	*m_right_fork;
	pthread_mutex_t	*m_output;
}	t_philo;

typedef struct s_philos {
	t_philo			*philos;
	struct timeval	start_time;
	int				*options;
	int				is_end;
	pthread_mutex_t	m_is_end;
	pthread_mutex_t	m_output;
	pthread_mutex_t	*m_forks;
}	t_philos;

int	is_num(char *str)
{
	while (*str && (*str >= '0' && *str <= '9'))
		str++;
	return (*str == '\0');
}

int	to_num(char *str)
{
	long	val;

	val = 0;
	while (*str)
	{
		if (val > val * 10 + ((*str) - '0'))
			return (-1);
		val = val * 10 + ((*str) - '0');
		if (val > __INT_MAX__)
			return (-1);
		str++;
	}
	return ((int) val);
}

int	*parse_args(int argc, char *argv[])
{
	int	*options;
	int	i;

	if (argc < 5 || argc > 6)
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
	i = 1;
	while (i < argc && is_num(argv[i]))
	{
		options[i - 1] = to_num(argv[i]);
		if (options[i - 1] <= 0)
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

int	create_mutexes(t_philos *philos)
{
	int	i;

	philos->m_forks = malloc(sizeof(*(philos->m_forks)) * philos->options[PHILOS_NUMBER]);
	if (philos->m_forks == NULL)
		return (1);
	if (pthread_mutex_init(&(philos->m_output), NULL))
		return (1);
	if (pthread_mutex_init(&(philos->m_is_end), NULL))
	{
		pthread_mutex_destroy(&(philos->m_output));
		return (1);
	}
	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		if (pthread_mutex_init(philos->m_forks + i, NULL))
		{
			while (i--)
				pthread_mutex_destroy(philos->m_forks + i);
			pthread_mutex_destroy(&(philos->m_output));
			pthread_mutex_destroy(&(philos->m_is_end));
			return (1);
		}
		i++;
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
	philo->m_right_fork = philos->m_forks + ((i + 1) % philos->options[PHILOS_NUMBER]);
	if (philo->m_right_fork == philo->m_left_fork)
		philo->m_right_fork = NULL;
	philo->m_output = &(philos->m_output);
	return (0);
}

void	destroy_philosopher(t_philo *philo)
{
	pthread_mutex_destroy(&(philo->m_last_eat_time));
	pthread_mutex_destroy(&(philo->m_eat_number));
}

int	create_philosophers(t_philos *philos)
{
	int	i;

	philos->philos = malloc(sizeof(*(philos->philos)) * philos->options[PHILOS_NUMBER]);
	if (philos->philos == NULL)
		return (1);
	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		if (create_philosopher(philos, i))
		{
			while (i--)
				destroy_philosopher(philos->philos + i);
			free(philos->philos);
			return (1);
		}
		i++;
	}
	return (0);
}

int	prepare_philosophers(t_philos *philos)
{
	if (create_mutexes(philos))
		return (1);
	if (create_philosophers(philos))
		return (1);
	// gettimeofday(&(philos->start_time), NULL);
	philos->is_end = 0;
	return (0);
}

int	philo_is_end(t_philo *philo)
{
	int	end;

	end = 0;
	pthread_mutex_lock(philo->m_is_end);
		end = philo->is_end[0];
	pthread_mutex_unlock(philo->m_is_end);
	return (end);
}

void	philo_write(t_philo *philo, char *str)
{
	pthread_mutex_lock(philo->m_output);
		printf("%d %d %s\n", get_ms_timestamp(philo->start_time[0]), philo->id, str);
	pthread_mutex_unlock(philo->m_output);
}

int	philo_eat(t_philo *philo)
{
	if (philo_is_end(philo))
		return (1);
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->m_left_fork);
		philo_write(philo, "has taken a fork");
		if (philo->m_right_fork)
			pthread_mutex_lock(philo->m_right_fork);
		else
			return (1);
		philo_write(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->m_right_fork);
		philo_write(philo, "has taken a fork");
		pthread_mutex_lock(philo->m_left_fork);
		philo_write(philo, "has taken a fork");
	}
	if (philo_is_end(philo))
	{
		pthread_mutex_unlock(philo->m_right_fork);
		pthread_mutex_unlock(philo->m_left_fork);
		return (1);
	}
	pthread_mutex_lock(&(philo->m_last_eat_time));
		philo->last_eat_time = get_ms_timestamp(philo->start_time[0]);
	pthread_mutex_unlock(&(philo->m_last_eat_time));
	philo_write(philo, "is eating");
	usleep(philo->options[EAT_TIME] * 1000);
	pthread_mutex_unlock(philo->m_right_fork);
	pthread_mutex_unlock(philo->m_left_fork);
	pthread_mutex_lock(&(philo->m_eat_number));
		philo->eat_number++;
	pthread_mutex_unlock(&(philo->m_eat_number));
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	if (philo_is_end(philo))
		return (1);
	philo_write(philo, "is sleeping");
	usleep(philo->options[SLEEP_TIME] * 1000);
	return (0);
}

int	philo_think(t_philo *philo)
{
	if (philo_is_end(philo))
		return (1);
	philo_write(philo, "is thinking");
	if (philo->options[DIE_TIME] > philo->options[EAT_TIME] + philo->options[SLEEP_TIME])
		usleep((philo->options[DIE_TIME] - philo->options[EAT_TIME] - philo->options[SLEEP_TIME]) * 250);
	return (0);
}

void	*philo_life(void *philo_raw)
{
	t_philo	*philo;

	philo = (t_philo *) philo_raw;
	if (philo->id % 2)
		usleep(philo->options[DIE_TIME] * 10);
	while (!philo_eat(philo) && !philo_sleep(philo) && !philo_think(philo));
	return (0);
}

int	run_simulation(t_philos *philos)
{
	int	i;

	gettimeofday(&(philos->start_time), NULL);
	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		philos->philos[i].thread = 0;
		if (pthread_create(&(philos->philos[i].thread), NULL, philo_life, philos->philos + i))
		{
			pthread_mutex_lock(&(philos->m_is_end));
				philos->is_end = 1;
			pthread_mutex_unlock(&(philos->m_is_end));
			while (i--)
				pthread_detach(philos->philos[i].thread);
			return (1);
		}
		i++;
	}
	return (0);
}

int	get_ms_timestamp(struct timeval start)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return ((now.tv_sec - start.tv_sec) * 1000 + (now.tv_usec - start.tv_usec) / 1000);
}

int	check_death(t_philo *philo)
{
	int		now;
	int		ret;

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
			printf("%d %d died\n", get_ms_timestamp(*(philo->start_time)), philo->id);
		pthread_mutex_unlock(philo->m_output);
	}
	return (ret);
}

int	check_ate_enough(t_philos *philos)
{
	int	i;
	int	all_ate;

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
	int	i;

	i = 0;
	while (!philos->is_end)
	{
		if (check_death(philos->philos + i) || check_ate_enough(philos))
			break ;
		i = (i + 1) % philos->options[PHILOS_NUMBER];
	}
}

void	clean_philos(t_philos *philos)
{
	int	i;

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
}

int	main(int argc, char *argv[])
{
	t_philos	philos;

	philos.options = parse_args(argc, argv);
	if (philos.options == NULL)
		return (1);
	if (prepare_philosophers(&philos))
		return (1);
	if (run_simulation(&philos))
	{
		clean_philos(&philos);
		return (1);
	}
	control_simulation(&philos);
	clean_philos(&philos);
	return (0);
}