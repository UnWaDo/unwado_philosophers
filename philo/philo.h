/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:56 by lalex             #+#    #+#             */
/*   Updated: 2022/07/01 22:39:56 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
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

typedef struct s_philo {
	pthread_t		thread;
	unsigned int	id;
	unsigned int	*options;
	struct timeval	*start_time;
	unsigned int	last_eat_time;
	pthread_mutex_t	m_last_eat_time;
	unsigned int	eat_number;
	pthread_mutex_t	m_eat_number;
	int				*is_end;
	pthread_mutex_t	*m_is_end;
	pthread_mutex_t	*m_is_start;
	pthread_mutex_t	*m_left_fork;
	pthread_mutex_t	*m_right_fork;
	pthread_mutex_t	*m_output;
}	t_philo;

typedef struct s_philos {
	t_philo			*philos;
	struct timeval	start_time;
	unsigned int	*options;
	int				is_end;
	pthread_mutex_t	m_is_end;
	pthread_mutex_t	m_is_start;
	pthread_mutex_t	m_output;
	pthread_mutex_t	*m_forks;
}	t_philos;

unsigned int	is_num(char *str);
int				to_num(char *str);
unsigned int	*parse_args(int argc, char *argv[]);
int				create_mutexes(t_philos *philos);
int				create_philosopher(t_philos *philos, int i);
void			destroy_philosopher(t_philo *philo);
int				create_philosophers(t_philos *philos);
int				prepare_philosophers(t_philos *philos);
int				philo_is_end(t_philo *philo);
int				philo_write(t_philo *philo, char *str);
int				philo_try_taking_fork(t_philo *philo, pthread_mutex_t *fork);
int				philo_take_forks(t_philo *philo);
int				philo_eat(t_philo *philo);
int				philo_sleep(t_philo *philo);
int				philo_think(t_philo *philo);
void			*philo_life(void *philo_raw);
int				run_simulation(t_philos *philos);
unsigned int	get_ms_timestamp(struct timeval start);
int				check_death(t_philo *philo);
int				check_ate_enough(t_philos *philos);
void			control_simulation(t_philos *philos);
void			clean_philos(t_philos *philos);

#endif