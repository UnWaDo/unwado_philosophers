/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:48 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 13:38:47 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*wait_all_full(void *philos_raw)
{
	uint		i;
	t_philos	*philos;

	i = 0;
	philos = (t_philos *) philos_raw;
	while (i < philos->options[PHILOS_NUMBER])
	{
		sem_wait(philos->s_full);
		i++;
	}
	sem_post(philos->s_is_end);
	return (0);
}

static void	make_all_full(t_philos *philos)
{
	uint	i;

	i = 0;
	while (i < philos->options[PHILOS_NUMBER])
	{
		sem_post(philos->s_full);
		i++;
	}
}

int	main(int argc, char *argv[])
{
	t_philos	philos;
	pthread_t	all_full;
	int			res;

	if (parse_args(argc, argv, philos.options))
		return (1);
	if (create_semaphores(&philos))
		return (1);
	res = run_simulation(&philos);
	if (res)
		sem_post(philos.s_is_end);
	if (philos.options[EAT_NUMBER])
	{
		pthread_create(&all_full, NULL, wait_all_full, &philos);
		sem_wait(philos.s_is_end);
		sem_post(philos.s_is_end);
		make_all_full(&philos);
		pthread_join(all_full, NULL);
	}
	clean_philos(&philos);
	return (res);
}
