NAME	=	philo
SRCS	=	main.c
HDRS	=	philo.h
INCL	=	$(addprefix -I, $(HDRS))
OBJS	=	$(SRCS:.c=.o)
CC		=	clang
CLFAGS	+=	-g -Wall -Wextra -Werror
.PHONY:		all clean fclean re

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CLFAGS) $(INCL) $(OBJS) -o $(NAME)

%.o:		%.c $(HDRS)
	$(CC) $(CLFAGS) $(ICNL) $< -c

re:			fclean all

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME)