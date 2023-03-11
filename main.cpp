/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:12:35 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/11 13:01:27 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/Irc.hpp"

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (-1);
	}
	else if (!isnumber(argv[1]))
	{
		std::cerr << "PORT should be a number" << std::endl;
		return (-1);
	}

	try
	{
		IrcServ irc(argv[1], argv[2]);
		irc.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (-1);
	}
	return (0);
}