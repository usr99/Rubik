/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 01:14:59 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "../include/FaceletCube.hpp"

FaceletCube::FaceletCube(const std::list<std::string>& scramble) :
	_corners {
		{ U9, R1, F3 },
		{ U7, F1, L3 },
		{ U1, L1, B3 },
		{ U3, B1, R3 },
		{ F9, R7, D3 },
		{ F7, D1, L9 },
		{ B9, L7, D7 },
		{ B7, R9, D9 }
	},
	_edges {
		{ U6, R2 },
		{ U8, F2 },
		{ U4, L2 },
		{ U2, B2 },
		{ D6, R8 },
		{ D2, F8 },
		{ D4, L8 },
		{ D8, R8 },
		{ F6, R4 },
		{ F4, L6 },
		{ B4, R6 },
		{ B6, L4 }
	},
	_data(FACELET_COUNT)
{
	// initialize a solved cube
	for (int i = 0; i < FACELET_COUNT; i++)
		_data[i] = static_cast<Facelet>(i);

	for (
		std::list<std::string>::const_iterator it = scramble.begin();
		it != scramble.end();
		it++
	) {
		try {
			const char	move	= it->at(0);
			int			factor	= 1;

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
				this->move(Rubik::SingmasterNotation.at(move), factor); // execute the move

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
	for (int i = 0; i < CORNER_COUNT; i++)
	{
		for (int j = 0; j < 3; j++)
			_corners[i][j] = rhs._corners[i][j];
	}

	for (int i = 0; i < EDGE_COUNT; i++)
	{
		for (int j = 0; j < 2; j++)
			_edges[i][j] = rhs._edges[i][j];
	}
	return *this;
}

void
FaceletCube::move(TurnAxis face, int factor)
{
	const std::vector<Facelet>& table	= Move::faceletTable.at(face);
	const std::vector<Facelet>	copy	= _data;

	for (int j = 0; j < factor; j++)
	{
		for (int i = 0; i < FACELET_COUNT; i++)
			_data[table[i]] = copy[i];
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
	for (int face = U; face <= B; face++)
	{
		const int*	start = coordinates[face];

		for (int y = start[1]; y < start[1] + 3; y++)
		{
			for (int x = start[0]; x < start[0] + 3; x++) {
				map[y][x] = Rubik::ColorScheme[_data[faceletIndex] / 9];
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
