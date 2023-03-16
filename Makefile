# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/24 14:09:59 by jtsizik           #+#    #+#              #
#    Updated: 2023/03/14 13:18:08 by jtsizik          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

HDRS =	src/CommandHandler.hpp \
		src/Server/IrcClient.hpp \
		src/Server/IrcServ.hpp \
		src/Command.hpp \
		src/Server/IrcChannel.hpp

OBJS =	obj/CommandHandler.o \
		obj/IrcClient.o \
		obj/IrcServ.o \
		obj/main.o \
		obj/Command.o \
		obj/IrcChannel.o

obj/%.o: src/%.cpp
	c++ -c -Wall -Wextra -Werror -std=c++98 $< -o $@

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