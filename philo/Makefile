NAME	=	philo
SRCS	=	pseudo.c
HDRS	=	philo.h
INCL	=	$(addprefix -I, $(HDRS))
OBJS	=	$(SRCS:.c=.o)
CC		=	clang
CLFAGS	+=	-Wall -Wextra -Werror
# DFLAGS	=	-g
DFLAGS	=	-fsanitize=thread
# DFLAGS	=	-fsanitize=address
.PHONY:		all clean fclean re

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CLFAGS) $(DFLAGS) $(INCL) $(OBJS) -o $(NAME)

%.o:		%.c $(HDRS)
	$(CC) $(CLFAGS) $(DFLAGS) $(ICNL) $< -c

re:			fclean all

clean:
	rm -f $(OBJS)

fclean:		clean
	rm -f $(NAME)