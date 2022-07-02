/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:56 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 14:17:39 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <semaphore.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>

# define HELP_USAGE	"Usage: %s number_of_philosophers \
time_to_die time_to_eat time_to_sleep [times_to_eat]\n"

enum	e_PARAMS
{
	PHILOS_NUMBER,
	DIE_TIME,
	EAT_TIME,
	SLEEP_TIME,
	EAT_NUMBER,
	PARAMS_NUMBER
};

typedef struct s_philos {
	struct timeval	start_time;
	uint			options[PARAMS_NUMBER];
	sem_t			*s_is_start;
	sem_t			*s_is_end;
	sem_t			*s_output;
	sem_t			*s_full;
	sem_t			*s_forks;
}	t_philos;

typedef struct s_philo
{
	uint			id;
	int				is_end;
	pthread_mutex_t	m_is_end;
	uint			last_eat_time;
	pthread_mutex_t	m_last_eat_time;
	uint			eat_number;
	t_philos		*philos;
}	t_philo;

# define SEM_START	"/START"
# define SEM_END	"/END"
# define SEM_OUTPUT	"/OUTPUT"
# define SEM_FULL	"/FULL"
# define SEM_FORKS	"/FORKS"

int		is_num(char *str);
uint	to_num(char *str);
int		parse_args(int argc, char *argv[], uint options[PARAMS_NUMBER]);
int		create_semaphores(t_philos *philos);
int		create_philosopher(t_philos *philos, int i);
void	destroy_philosopher(t_philo *philo);
int		prepare_philosophers(t_philos *philos);
int		philo_is_end(t_philo *philo);
int		philo_write(t_philo *philo, char *str);
int		philo_eat(t_philo *philo);
int		philo_sleep(t_philo *philo);
int		philo_think(t_philo *philo);
void	*philo_life(void *philo_raw);
int		run_simulation(t_philos *philos);
uint	get_ms_timestamp(struct timeval start);
void	control_simulation(t_philos *philos);
void	clean_philos(t_philos *philos);
void	close_semaphore(sem_t *sem, char *name);
void	ft_mssleep(uint duration);
void	*philo_control(void *philo_raw);

#endif