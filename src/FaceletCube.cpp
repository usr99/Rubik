/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/05/18 23:24:20 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/FaceletCube.hpp"
#include "../include/CubieCube.hpp"
#include "../include/permutations.hpp"

FaceletCube::FaceletCube(const std::list<std::string>& sequence)
{
	// initialize a solved cube
	for (int i = 0; i < FACELET_COUNT; i++)
		_facelets[i] = static_cast<Facelet>(i);

	this->scramble(sequence);
}

FaceletCube::FaceletCube(const std::array<Facelet, FACELET_COUNT>& facelets) :
	_facelets(facelets) {}

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
	try {
		const std::vector<Facelet> &table = Rubik::faceletTable.at(face);

		for (int j = 0; j < factor; j++)
		{
			const std::array<Facelet, FACELET_COUNT> copy(_facelets);
			for (int i = 0; i < FACELET_COUNT; i++)
				_facelets[table[i]] = copy[i];
		}
	} catch (const std::out_of_range &e) {
		throw std::invalid_argument(std::string("Accepted moves are U, R, F, D, L and B"));
	}
}

void
FaceletCube::render() const
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
	for (int face = 0; face < FACES_COUNT; face++)
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
	std::cout << "\n";
}

std::array<CornerCubie, CORNER_COUNT>
FaceletCube::getCornerCubies() const
{
	return _ConvertCubies<CornerCubie, Corner, CORNER_COUNT, 3>(Rubik::CornerFacelets);
}

std::array<EdgeCubie, EDGE_COUNT>
FaceletCube::getEdgeCubies() const
{
	return _ConvertCubies<EdgeCubie, Edge, EDGE_COUNT, 2>(Rubik::EdgeFacelets);
}

const std::array<Facelet, FACELET_COUNT>&
FaceletCube::getFacelets() const
{
	return _facelets;
}

template <typename T, typename U, unsigned int count, unsigned int stickers_count>
std::array<T, count>
FaceletCube::_ConvertCubies(const Facelet reference[count][stickers_count]) const
{
	std::array<T, count> cubies;
	std::vector<bool>	 cubieFound(count, false);

	for (unsigned int i = 0; i < count; i++)
	{
		/* Find the color of the facelets composing the cubie */
		std::array<Facelet, stickers_count> cubie;
		for (unsigned int j = 0; j < stickers_count; j++)
		{
			const int faceIdx = reference[i][j] / 9;
			const int faceletIdx = reference[i][j] % 9;
			cubie[j] = static_cast<Facelet>(_facelets[faceIdx * 9 + faceletIdx] / 9);
		}

		/* Find the name of the cubie having the same colors */
		int name = -1;
		char orientation;
		for (unsigned int n = 0; n < count; n++)
		{
			/* Search for the facelet that matches the first facelet of the reference cubie */
			unsigned int faceletIdx = 0;
			while (reference[n][0] / 9 != cubie[faceletIdx] && faceletIdx < stickers_count)
				faceletIdx++;

			/* One facelet match ! */
			if (faceletIdx != stickers_count)
			{
				unsigned int matching = 1;
				unsigned int refIdx = 1;
				orientation = faceletIdx;

				/* Count the number of following facelets that also match */
				faceletIdx = (faceletIdx + 1) % stickers_count;
				while (reference[n][refIdx] / 9 == cubie[faceletIdx] && refIdx < stickers_count)
				{
					matching++;
					refIdx++;
					faceletIdx = (faceletIdx + 1) % stickers_count;
				}

				/* All facelets match ! Cubie found ! */
				if (matching == stickers_count)
				{
					name = n;
					cubieFound[n] = true;
					break ;
				}
			}
		}

		if (name == -1)
			throw std::runtime_error("Invalid cubie found");

		cubies[i] = { static_cast<U>(name), orientation };
	}

	/* Check that each cubie is unique */
	if (std::find(cubieFound.begin(), cubieFound.end(), false) != cubieFound.end())
		throw std::runtime_error("Duplicate cubies found");

	/* Check the orientation */
	int sum = 0;
	for (auto cubie = cubies.begin(); cubie != cubies.end(); cubie++) 
		sum += cubie->o;
	if (sum % stickers_count != 0)
		throw std::runtime_error("Cubies orientation is not valid");

	return cubies;
}
