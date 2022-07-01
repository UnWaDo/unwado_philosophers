#include "philo.h"

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
