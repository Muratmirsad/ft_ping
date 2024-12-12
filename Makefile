NAME = ft_ping
FLAGS = -Wall -Wextra -Werror
SRCS = ft_ping.c is_valid_funcs.c flags.c

all: $(NAME)

$(NAME): $(SRCS)
	@gcc $(FLAGS) $(SRCS) -o $(NAME)
	@clear
	@echo "program created."

clean:
	@rm -rf $(NAME)
	@echo "program cleaned."

fclean: clean

re: clean all