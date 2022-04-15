/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 02:15:25 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/15 03:32:38 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Solver.hpp"

Solver::Solver(const std::list<std::string>& scramble) : _cube(scramble) {}

/*
** Print in the terminal a 2D representation of the cube state
*/
void
Solver::render()
{
	int	map[9][12]			= { 0 };
	int	coordinates[6][2]	= {	// coordinates of the upper-left corner of each face on the map
		{ 3, 0 }, // UP
		{ 0, 3 }, // LEFT
		{ 3, 3 }, // FRONT
		{ 6, 3 }, // RIGHT
		{ 9, 3 }, // BACK
		{ 3, 6 }, // DOWN
	};

	// draw each face on the map
	for (int face = Face::UP; face <= Face::DOWN; face++)
	{
		const int*	start		= coordinates[face];
		int			cubieIndex	= 0;

		for (int y = start[1]; y < start[1] + 3; y++)
		{
			for (int x = start[0]; x < start[0] + 3; x++) {
				map[y][x] = _cube.cubies[face][cubieIndex];
				cubieIndex++;
			}
		}
	}

	// print the map
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 12; j++)
			std::cout << "\e[48;5;" << map[i][j] << "m ";
		std::cout << "\e[0m\n";
	}
	std::cout << "=====================================\n";
}
