/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/01 02:28:04 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include "../include/CoordCube.hpp"

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

		CoordCube c1(scramble);
		// CubieCube c2(scramble);

		// std::time_t start = std::time(&start);
		// std::list<std::string> solution = c1.solve();
		// std::time_t end = std::time(&end);
		
		// c2.toFacelet().render();
		// c2.scramble(solution);

		// for ( // print solution
		// 	std::list<std::string>::const_iterator it = solution.begin();
		// 	it != solution.end();
		// 	it++
		// ) {
		// 	std::cout << *it << " ";
		// }
		// std::cout << "\n";
		// std::cout << "MOVES: " << solution.size() << "\n";
		// std::cout << "TIME: " << std::difftime(end, start) << "s\n";

		return (0);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return (1);
	}
}
