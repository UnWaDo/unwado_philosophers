NAME	=	philo
SRCS	=	main.c initializer.c numeric.c philo_eat.c \
				philo_other.c philos_control.c additional.c \
					philos_operations.c
HDRS	=	philo.h
INCL	=	$(addprefix -I, $(HDRS))
OBJS	=	$(SRCS:.c=.o)
# CC		=	clang
CLFAGS	+=	-Wall -Wextra -Werror
# DFLAGS	=	-g
DFLAGS	=	-fsanitize=thread
# DFLAGS	=	-fsanitize=address
LFLAGS	=	-pthread
.PHONY:		all clean fclean re

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CLFAGS) $(DFLAGS) $(LFLAGS) $(INCL) $(OBJS) -o $(NAME)

%.o:		%.c $(HDRS)
	$(CC) $(CLFAGS) $(DFLAGS) $(LFLAGS) $(ICNL) $< -c

re:			fclean all

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME)