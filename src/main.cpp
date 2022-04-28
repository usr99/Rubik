/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/28 19:29:08 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

#if 1

		CoordCube c1(scramble);
		CubieCube c2(scramble);

		std::list<std::string> solution = c1.solve();
		c2.scramble(solution);
		c2.toFacelet().render();

		for ( // print solution
			std::list<std::string>::const_iterator it = solution.begin();
			it != solution.end();
			it++
		) {
			std::cout << *it << " ";
		}
		std::cout << "\n";

#else

		CubieCube c(scramble);
		CoordCube d(c);

		std::cout	<< c.getCornerOriCoord() << "\t"
					<< c.getEdgeOriCoord() << "\t"
					<< c.getUDSliceCoord() << "\t"
					<< c.getCornerPermCoord() << "\t"
					<< c.getPhase2EdgePermCoord() << "\t"
					<< c.getPhase2UDSliceCoord() << "\n";
		std::cout	<< d._cornersOri << "\t"
					<< d._edgesOri << "\t"
					<< d._UDSlice << "\t"
					<< d._cornersPerm << "\t"
					<< d._edgesPermP2 << "\t"
					<< d._UDSliceP2 << "\n\n";

		while (!std::cin.eof())
		{
			std::string move;

			std::cin >> move;

			int factor = 1;
			if (move.back() == '2')
				factor = 2;
			else if (move.back() == '\'')
				factor = 3;

			c.move(move.front(), factor);
			d.move(move.front(), factor);
			
			std::cout	<< c.getCornerOriCoord() << "\t"
						<< c.getEdgeOriCoord() << "\t"
						<< c.getUDSliceCoord() << "\t"
						<< c.getCornerPermCoord() << "\t"
						<< c.getPhase2EdgePermCoord() << "\t"
						<< c.getPhase2UDSliceCoord() << "\n";
			std::cout	<< d._cornersOri << "\t"
						<< d._edgesOri << "\t"
						<< d._UDSlice << "\t"
						<< d._cornersPerm << "\t"
						<< d._edgesPermP2 << "\t"
						<< d._UDSliceP2 << "\n\n";
		}

#endif

		return (0);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return (1);
	}
}
