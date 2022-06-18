#include <printf.h>
#include <pthread.h>
#include <sys/time.h>

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
	int				philo_id;
	int				eat_number;
	struct timeval	finished_eating;

}	t_philo;

typedef struct s_philos {
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	output;
	int				*options;
	struct timeval	start;
}	t_philos;

int	is_num(char *str)
{
	while (*str && (*str >= '0' && *str <= '9'))
		str++;
	return (*str == '\0');
}

int	*parse_args(int argc, char *argv[])
{
	int	*options;
	int	i;

	if (argc < 5 || argc > 6)
	{
		printf("Error: invalid number of arguments\n");
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [times_to_eat]\n", argv[0]);
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
		options[i - 1] = to_int(argv[i]);
		if (options[i - 1] <= 0)
			break ;
		i++;
	}
	if (i < argc)
	{
		printf("Error: arguments are invalid or not numeric\n");
		printf("Usage: %s number_of_philosophers time_to_die time_to_eat time_to_sleep [times_to_eat]\n", argv[0]);
		return (NULL);
	}
	if (argc - 1 < PARAMS_NUMBER)
		options[PARAMS_NUMBER - 1] = 0;
	return(options);
}

t_philos	*create_philosophers(int *options);

int	main(int argc, char *argv[])
{
	int	*options;
	t_philos	*philo;

	options = parse_args(argc, argv);
	if (!options)
		return (1);
	philo = create_philosophers(options);
}