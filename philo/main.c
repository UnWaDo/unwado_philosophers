/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lalex <lalex@student.21-school.ru>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/01 22:39:48 by lalex             #+#    #+#             */
/*   Updated: 2022/07/02 05:57:42 by lalex            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char *argv[])
{
	t_philos	philos;

	if (parse_args(argc, argv, philos.options))
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
