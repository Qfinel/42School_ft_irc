# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 14:09:59 by jtsizik           #+#    #+#              #
#    Updated: 2023/03/12 15:07:18 by jtsizik          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

# HDRS =	

OBJS =	obj/main.o

obj/%.o: src/%.cpp
	c++ -c -Wall -Wextra -Werror -std=c++98 $< -o $@

$(NAME): obj $(OBJS)
	c++ -Wall -Wextra -Werror -std=c++98 $(OBJS) -o $(NAME)

all: $(NAME)

obj:
	mkdir obj

install:


clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all