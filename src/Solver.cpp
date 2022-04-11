/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 02:15:25 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/11 20:55:09 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "../include/Solver.hpp"

Solver::Solver()
{
	Color::iterator it = Color::begin();

	// set the cube in a solved state
	for (int face = 0; face < 6; face++)
	{
		for (int cubie = 0; cubie < 9; cubie++)
			_cubies[face][cubie] = *it;
		++it; // iterate over all color values
	}
}

void printFace(int map[9][12], int x, int y, int face[9])
{
	int cubie = 0;

	for (int i = y; i < y + 3; i++)
	{
		for (int j = x; j < x + 3; j++)
		{
			map[i][j] = face[cubie];
			cubie++;
		}
	}
}

/*
** Print in the terminal a 2D representation of the cube state
*/
void Solver::render()
{
	int	map[9][12] = { 0 };

	printFace(map, 3, 0, _cubies[0]); // UP
	printFace(map, 0, 3, _cubies[1]); // LEFT
	printFace(map, 3, 3, _cubies[2]); // FRONT
	printFace(map, 6, 3, _cubies[3]); // RIGHT
	printFace(map, 9, 3, _cubies[4]); // BACK
	printFace(map, 3, 6, _cubies[5]); // DOWN

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 12; j++)
			std::cout << "\e[48;5;" << map[i][j] << "m ";
		std::cout << "\e[0m\n";
	}
}
