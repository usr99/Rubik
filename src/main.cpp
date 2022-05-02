/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 17:31:12 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <csignal>
#include "../include/Solver.hpp"

// protoypes
std::list<std::string>	solve(const std::list<std::string>& sequence);
std::list<std::string>	solve(const CubieCube& cube);

void sig_handler(int sig)
{
	std::cout	<< "Received signal " << sig
				<< "\nTerminating...\n";

	MoveTables::destroy();
	PruningTables::destroy();

	exit(EXIT_FAILURE);
}

int main(int ac, char **av)
{
	try
	{
		if (ac < 2)
			throw std::invalid_argument("No scramble provided");

		if (
			signal(SIGINT, sig_handler) == SIG_ERR ||
			signal(SIGTERM, sig_handler) == SIG_ERR
		)	throw std::runtime_error("Setting signal handlers failed");

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

		CubieCube cube(scramble); // create a cube from the scramble

		// solve the cube
		std::time_t start = std::time(&start);
		std::list<std::string> solution = solve(cube);
		std::time_t end = std::time(&end);

		// print results
		cube.toFacelet().render(); // render the scramble
		std::cout << "Solution: ";
		for (
			std::list<std::string>::const_iterator it = solution.begin();
			it != solution.end();
			it++
		)	std::cout << *it << " ";
		std::cout << "\nMOVES: " << solution.size() << "\n";
		std::cout << "TIME: " << std::difftime(end, start) << "s\n";

		// free tables
		MoveTables::destroy();
		PruningTables::destroy();

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

	// Modify the moves transitioning between the two phases if they're on the same face
	if (solution.back().front() == tmp.front().front())
	{
		const std::vector<char>	values	= { '\0', '2', '\'' };
		char						v1		= 1;
		char						v2		= 1;
		char						move	= solution.back().front();

		if (solution.back().size() > 1)
			v1 = (std::find(values.begin(), values.end(), solution.back()[1]) - values.begin()) + 1;
		if (tmp.front().size() > 1)
			v2 = (std::find(values.begin(), values.end(), tmp.front()[1]) - values.begin()) + 1;

		// erase the moves from the solution
		solution.erase(--solution.end());
		tmp.erase(tmp.begin());

		v1 = (v1 + v2) % 4;
		if (v1) // the moves do not cancel each other, they add up
			solution.push_back(std::string(1, move) + values[v1 - 1]);
	}
	solution.insert(solution.end(), tmp.begin(), tmp.end());

	return solution;
}
