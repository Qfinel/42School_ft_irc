# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 14:09:59 by jtsizik           #+#    #+#              #
#    Updated: 2023/03/12 16:02:18 by jtsizik          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

HDRS =	src/Server/IrcClient.hpp \
		src/Server/IrcServ.hpp	

OBJS =	obj/IrcClient.o \
		obj/IrcServ.o \
		obj/main.o

obj/%.o: src/Server/%.cpp
	c++ -c -Wall -Wextra -Werror -std=c++98 $< -o $@

obj/%.o: %.cpp
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