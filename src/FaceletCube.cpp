/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 01:44:39 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../include/FaceletCube.hpp"

FaceletCube::FaceletCube(const std::list<std::string>& scramble) :
	_facelets(FACELET_COUNT)
{
	// initialize a solved cube
	for (int i = 0; i < FACELET_COUNT; i++)
		_facelets[i] = static_cast<Facelet>(i);

	for (
		std::list<std::string>::const_iterator it = scramble.begin();
		it != scramble.end();
		it++
	) {
		try {
			const char	singmaster	= it->at(0);
			int			factor		= 1;

			if (it->length() == 2)
			{
				if (it->at(1) == '2')
					factor = 2; // half-turn
				else if (it->at(1) == '\'')
					factor = 3; // counter clockwise
				else
					throw std::exception(); // not an existing move
			}
			else if (it->length() != 1)
				throw std::exception(); // not an existing move

			for (int i = 0; i < factor; i++)
				this->move(singmaster, factor); // execute the move

		} catch (const std::exception& e) {
			throw std::invalid_argument("Scramble is not valid.");
		}
	}
}

FaceletCube::FaceletCube(const FaceletCube& rhs)
{
	*this = rhs;
}

FaceletCube::~FaceletCube() {}

FaceletCube&
FaceletCube::operator=(const FaceletCube& rhs)
{
	_facelets = rhs._facelets;
	return *this;
}

void
FaceletCube::move(char face, int factor)
{
	const std::vector<Facelet>& table	= Move::faceletTable.at(face);
	const std::vector<Facelet>	copy	= _facelets;

	for (int j = 0; j < factor; j++)
	{
		for (int i = 0; i < FACELET_COUNT; i++)
			_facelets[table[i]] = copy[i];
	}
}

void
FaceletCube::render()
{
	int		faceletIndex		= 0;
	int		map[9][12]			= {0};
	int		coordinates[6][2]	= {	// coordinates of the upper-left corner of each face on the map
			{ 3, 0 }, // UP
			{ 6, 3 }, // RIGHT
			{ 3, 3 }, // FRONT
			{ 3, 6 }, // DOWN
			{ 0, 3 }, // LEFT
			{ 9, 3 }, // BACK
	};

	// draw each face on the map
	for (int face = 0; face < 6; face++)
	{
		const int*	start = coordinates[face];

		for (int y = start[1]; y < start[1] + 3; y++)
		{
			for (int x = start[0]; x < start[0] + 3; x++) {
				map[y][x] = Rubik::ColorScheme[_facelets[faceletIndex] / 9];
				faceletIndex++;
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
