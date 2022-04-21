/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/22 00:31:03 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>
#include <sstream>

#include "../include/FaceletCube.hpp"

int main(int ac, char **av)
{
	try
	{
		if (ac < 2)
			throw std::invalid_argument("A scramble is needed");

		std::list<std::string>	scramble;

		// read scramble from arguments
		for (int i = 1; i < ac; i++)
		{
			std::istringstream	iss(av[i]);
			std::string			buf;

			while(std::getline(iss, buf, ' '))
				scramble.push_back(buf);
		}

		FaceletCube cube(scramble);

		cube.render();

		return (0);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl; 
		return (1);
	}
}
