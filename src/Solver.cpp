/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 02:15:25 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/12 10:44:35 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <algorithm>
#include "../include/Solver.hpp"

Solver::Solver()
{
	// set the cube in a solved state
	for (Color::iterator it = Color::begin(); it != Color::end(); ++it)
		_cubies.emplace_back(std::vector<int>(9, *it));
}

void printFace(int map[9][12], int x, int y, std::vector<int>& face)
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
	std::cout << "=====================================\n";
}

std::vector<int> swapVertical(std::vector<int>& face, std::vector<int> row, int start)
{
	std::vector<int>	buffer;
	int j = start;

	for (int i = 0; i < 3; i++)
	{
		buffer.emplace_back(face[j]);
		face[j] = row[i];
		j += 3;
	}
	return buffer;
}

std::vector<int> swapHorizontal(std::vector<int>& face, std::vector<int> row, int start)
{
	std::vector<int>	buffer;

	for (int i = 0; i < 3; i++)
	{
		const int index = start * 3 + i;
		buffer.emplace_back(face[index]);
		face[index] = row[i];
	}
	return buffer;
}

void Solver::moveRight()
{
	std::vector<int>	tmp({
		_cubies[FRONT][2],
		_cubies[FRONT][5],
		_cubies[FRONT][8]
	});

	_rotateFace(_cubies[RIGHT]);
	tmp = swapVertical(_cubies[UP], tmp, 2);
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapVertical(_cubies[BACK], tmp, 0);
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapVertical(_cubies[DOWN], tmp, 2);
	tmp = swapVertical(_cubies[FRONT], tmp, 2);
}

void Solver::moveLeft()
{
	std::vector<int>	tmp({
		_cubies[FRONT][0],
		_cubies[FRONT][3],
		_cubies[FRONT][6]
	});

	_rotateFace(_cubies[LEFT]);
	tmp = swapVertical(_cubies[DOWN], tmp, 0);
	tmp = swapVertical(_cubies[BACK], tmp, 2);
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapVertical(_cubies[UP], tmp, 0);
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapVertical(_cubies[FRONT], tmp, 0); 
}

void Solver::moveUp()
{
	std::vector<int>	tmp({
		_cubies[FRONT][0],
		_cubies[FRONT][1],
		_cubies[FRONT][2]
	});

	_rotateFace(_cubies[UP]);
	tmp = swapHorizontal(_cubies[LEFT], tmp, 0);
	tmp = swapHorizontal(_cubies[BACK], tmp, 0);
	tmp = swapHorizontal(_cubies[RIGHT], tmp, 0);
	tmp = swapHorizontal(_cubies[FRONT], tmp, 0);
}

void Solver::moveDown()
{
	std::vector<int>	tmp({
		_cubies[FRONT][6],
		_cubies[FRONT][7],
		_cubies[FRONT][8]
	});

	_rotateFace(_cubies[DOWN]);
	tmp = swapHorizontal(_cubies[RIGHT], tmp, 2); 
	tmp = swapHorizontal(_cubies[BACK], tmp, 2);
	tmp = swapHorizontal(_cubies[LEFT], tmp, 2);
	tmp = swapHorizontal(_cubies[FRONT], tmp, 2);
}

void Solver::moveFront()
{
	std::vector<int>	tmp({
		_cubies[UP][6],
		_cubies[UP][7],
		_cubies[UP][8]
	});

	_rotateFace(_cubies[FRONT]);
	tmp = swapVertical(_cubies[RIGHT], tmp, 0); 
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapHorizontal(_cubies[DOWN], tmp, 0);
	tmp = swapVertical(_cubies[LEFT], tmp, 2);
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapHorizontal(_cubies[UP], tmp, 2);
}

void Solver::moveBack()
{
	std::vector<int>	tmp({
		_cubies[UP][0],
		_cubies[UP][1],
		_cubies[UP][2]
	});

	_rotateFace(_cubies[BACK]);
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapVertical(_cubies[LEFT], tmp, 0); 
	tmp = swapHorizontal(_cubies[DOWN], tmp, 2);
	std::reverse(tmp.begin(), tmp.end());
	tmp = swapVertical(_cubies[RIGHT], tmp, 2);
	tmp = swapHorizontal(_cubies[UP], tmp, 0);
}

void Solver::_rotateFace(std::vector<int>& face)
{
	std::vector<int> copy(face);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			face[i * 3 + j] = copy[((2 - j) * 3) + i];
	}
}
