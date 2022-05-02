/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 15:41:16 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include "../include/Solver.hpp"

// protoypes
std::list<std::string>	solve(const std::list<std::string>& sequence);
std::list<std::string>	solve(const CubieCube& cube);

int main(int ac, char **av)
{
	try
	{
		if (ac < 2)
			throw std::invalid_argument("No scramble provided");

		// read scramble from arguments
		std::list<std::string> scramble;
		for (int i = 1; i < ac; i++)
		{
			std::istringstream	iss(av[i]);
			std::string			buf;

			while(std::getline(iss, buf, ' '))
			{
				if (buf.size() != 0) // if the scramble contains consecutives spaces, buf can be empty
				scramble.push_back(buf);
		}
		}
		if (!scramble.size())
			throw std::invalid_argument("No scramble provided");

		CubieCube cube(scramble);	// create a cube from the scramble
		cube.toFacelet().render();	// render the scramble

		// solve the cube
		std::time_t start = std::time(&start);
		std::list<std::string> solution = solve(cube);
		std::time_t end = std::time(&end);

		// print results
		std::cout << "Solution: ";
		for (
			std::list<std::string>::const_iterator it = solution.begin();
			it != solution.end();
			it++
		)	std::cout << *it << " ";
		std::cout << "\nMOVES: " << solution.size() << "\n";
		std::cout << "TIME: " << std::difftime(end, start) << "s\n";

		// free tables
		delete mt;
		delete prun;

		return (0);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return (1);
	}
}

std::list<std::string>	solve(const std::list<std::string>& sequence)
{
	return solve(CubieCube(sequence));
}

std::list<std::string> solve(const CubieCube& cube)
{
	// Solve phase 1
	Solver<CubeStateP1>		solverP1;
	std::list<std::string>	solution = solverP1.solve(CubeStateP1(cube));

	// Apply the solution found to a copy of the cube
	CubieCube copy(cube);
	copy.scramble(solution);

	// Solve phase 2 from the new state
	Solver<CubeStateP2>		solverP2;
	std::list<std::string>	tmp = solverP2.solve(CubeStateP2(copy));
	solution.insert(solution.end(), tmp.begin(), tmp.end());

	return solution;
}
