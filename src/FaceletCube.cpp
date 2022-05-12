/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/05/11 21:51:18 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/FaceletCube.hpp"
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

const std::array<Facelet, FACELET_COUNT>&
FaceletCube::getFacelets() const
{
	return _facelets;
}
