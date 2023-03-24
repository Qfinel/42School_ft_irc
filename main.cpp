/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtsizik <jtsizik@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 14:12:35 by jtsizik           #+#    #+#             */
/*   Updated: 2023/03/24 12:27:42 by jtsizik          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/Server/IrcServ.hpp"

bool is_number(const std::string& str)
{
    std::string::const_iterator it = str.begin();
    while (it != str.end() && std::isdigit(*it))
		it++;
    return (it == str.end());
}

int	main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return (-1);
	}
	else if (!is_number(argv[1]))
	{
		std::cerr << "PORT should be a non-negative number" << std::endl;
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
	system("leaks ircserv");
	return (0);
}