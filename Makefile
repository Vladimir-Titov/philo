NAME	=	philo

SRC		=	action.c	main.c	msg.c\
			philo.c	thread.c	time.c\
			utils.c\

SRCS	=	$(addprefix src/, $(SRC))

OBJ		=	$(SRCS:.c=.o)

CFLAGS	=	-Wall -Wextra -Werror -g -fsanitize=thread 

F_THREAD	=	-pthread

.PHONY	:	all clean fclean re debug

all		:	$(NAME)

$(NAME)	:	$(OBJ)
	$(CC) -o $(NAME) $(F_THREAD) $(CFLAGS) $(OBJ)

%.o		:	%.c philo.h
	$(CC) -c $(F_THREAD) $(CFLAGS) $< -o $@

clean	:
	rm -f $(OBJ)

fclean	:	clean
	rm -f $(NAME)

re		:	fclean all