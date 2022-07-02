/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:46 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 13:04:08 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	create_semaphore(sem_t **sem, char *name, unsigned int val)
{
	*sem = sem_open(name, O_CREAT | O_EXCL, 0644, val);
	if (*sem != SEM_FAILED)
		return (0);
	if (sem_unlink(name))
		return (1);
	*sem = sem_open(name, O_CREAT | O_EXCL, 0644, val);
	if (*sem != SEM_FAILED)
		return (0);
	return (1);
}

void	close_semaphore(sem_t *sem, char *name)
{
	if (sem != SEM_FAILED)
		sem_close(sem);
	sem_unlink(name);
}

int	create_semaphores(t_philos *philos)
{
	if (create_semaphore(&(philos->s_forks),
			SEM_FORKS, philos->options[PHILOS_NUMBER])
		|| create_semaphore(&(philos->s_is_start), SEM_START, 0)
		|| create_semaphore(&(philos->s_is_end), SEM_END, 0)
		|| create_semaphore(&(philos->s_output), SEM_OUTPUT, 1)
		|| create_semaphore(&(philos->s_full), SEM_FULL, 0))
	{
		close_semaphore(philos->s_forks, SEM_FORKS);
		close_semaphore(philos->s_is_start, SEM_START);
		close_semaphore(philos->s_is_end, SEM_END);
		close_semaphore(philos->s_output, SEM_OUTPUT);
		close_semaphore(philos->s_full, SEM_FULL);
		printf("Error: system is overloaded or some \
semaphores are already initialized. Restart should help\n");
		return (1);
	}
	return (0);
}
