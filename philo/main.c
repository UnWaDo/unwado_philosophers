#include "philo.h"
#include <pthread.h>
#include <unistd.h>

typedef	struct s_options
{
	int	amount;
	int	ttd;
	int	tte;
	int	tts;
	int	eat_n
}	t_options;

typedef	struct s_philo
{
	pthread_t	thread;
	int			philo_n;
	int			status;
	int			left_fork;
	int			death_timestamp;
	t_philo 	*next_philo
}	t_philo;

typedef	struct s_philo_arg
{
	t_options	*options;
	t_philo		*philo;
}	t_philo_arg;

void	*philosopher(void *philo_arg)
{
	t_philo		*philo;
	t_options	*options;

	philo = ((t_philo_arg *)philo_arg)->philo;
	options = ((t_philo_arg *)philo_arg)->options;
	printf("%d %d is created\n");

	return (NULL);
}

int	atoi(char *str)
{
	int	v;

	v = -1;
	while (*str && *str == ' ')
		str++;
	while (*str && *str <= '9' && *str >= '0')
	{
		v = 10 * v + (*str - '0');
		str++;
	}
	return (v);
}

t_options	init_options(int argc, char *argv[])
{
	t_options	options;

	if (argc < 5)
		exit(EXIT_FAILURE);
	options.amount = atoi(argv[1]);
	options.ttd = atoi(argv[2]);
	options.tte = atoi(argv[3]);
	options.tts = atoi(argv[4]);
	if (argc > 5)
		options.eat_n = atoi(argv[5]);
	else
		options.eat_n = 0;
	if (options.amount <= 0 || options.ttd <= 0 || options.tte <= 0 || options.tts <= 0 || options.eat_n < 0)
		exit(EXIT_FAILURE);
	return (options);
} 

int	main(int argc, char *argv[])
{
	t_options	options;
	pthread_t	*threads;
	int			philo_n;
	int			i;

	options = init_options(argc, argv);
	threads = malloc(philo_n * sizeof(*threads));
	if (threads == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < philo_n)
	{
		if (pthread_create(threads + i, NULL, &philosopher, &philo))
			exit(EXIT_FAILURE);
		i++;
	}
	exit(EXIT_SUCCESS);
}