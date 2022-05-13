/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubieCube.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:52:27 by user42            #+#    #+#             */
/*   Updated: 2022/05/13 17:55:28 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>

#include "../include/CubieCube.hpp"
#include "../include/permutations.hpp"

CubieCube::CubieCube(const std::list<std::string>& sequence)
{
	// set the cube in a solved state
	for (int i = 0; i < CORNER_COUNT; i++)
	{
		_corners[i].c = static_cast<Corner>(i);
		_corners[i].o = 0;
	}
	for (int i = 0; i < EDGE_COUNT; i++)
	{
		_edges[i].e = static_cast<Edge>(i);
		_edges[i].o = 0;
	}

	this->scramble(sequence);
}

CubieCube::CubieCube(
	const std::array<CornerCubie, CORNER_COUNT>& c,
	const std::array<EdgeCubie, EDGE_COUNT>& e
)	: _corners(c), _edges(e) {}

CubieCube::CubieCube(const CubieCube& rhs)
{
	*this = rhs;
}

CubieCube::~CubieCube() {}

CubieCube&
CubieCube::operator=(const CubieCube& rhs)
{
	for (int i = 0; i < CORNER_COUNT; i++)
	{
		_corners[i].c = rhs._corners[i].c;
		_corners[i].o = rhs._corners[i].o;
	}

	for (int i = 0; i < EDGE_COUNT; i++)
	{
		_edges[i].e = rhs._edges[i].e;
		_edges[i].o = rhs._edges[i].o;
	}
	return *this;
}

bool
CubieCube::operator==(const CubieCube& rhs)
{
	for (int i = 0; i < CORNER_COUNT; i++)
	{
		if (
			_corners[i].c != rhs._corners[i].c ||
			_corners[i].o != rhs._corners[i].o
		) return false;
	}

	for (int i = 0; i < EDGE_COUNT; i++)
	{
		if (
			_edges[i].e != rhs._edges[i].e ||
			_edges[i].o != rhs._edges[i].o
		) return false;		
	}
	return true;
}

void
CubieCube::move(char face, int factor)
{
	try {
		const std::vector<CornerCubie>&	cTable(Rubik::cornerTable.at(face));
		const std::vector<EdgeCubie>&	eTable(Rubik::edgeTable.at(face));

		for (int j = 0; j < factor; j++)
		{
			const CubieCube copy(*this);

			for (int i = 0; i < CORNER_COUNT; i++)
			{
				const CornerCubie& newCorner = copy._corners[cTable[i].c];

				_corners[i].c = newCorner.c;
				_corners[i].o = (newCorner.o + cTable[i].o) % 3;
			} 
			for (int i = 0; i < EDGE_COUNT; i++)
			{
				const EdgeCubie& newEdge = copy._edges[eTable[i].e];

				_edges[i].e = newEdge.e;
				_edges[i].o = (newEdge.o + eTable[i].o) % 2;
			}
		}
	} catch (const std::out_of_range& e) {
		throw std::invalid_argument("Accepted moves are U, R, F, D, L and B");
	}
}

FaceletCube
CubieCube::toFacelet()
{
	std::array<Facelet, FACELET_COUNT> facelets;

	for (int i = 0; i < FACES_COUNT; i++)
	{
		const int center = i * 9 + 4; // centers are X4 facelets where X is the face
		facelets[center] = static_cast<Facelet>(center);
	}

	for (int i = 0; i < CORNER_COUNT; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			const int newOri = (j - _corners[i].o + 3) % 3;
			facelets[Rubik::CornerFacelets[i][j]] = Rubik::CornerFacelets[_corners[i].c][newOri];
		} 
	}

	for (int i = 0; i < EDGE_COUNT; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			const int newOri = (j - _edges[i].o + 2) % 2;
			facelets[Rubik::EdgeFacelets[i][j]] = Rubik::EdgeFacelets[_edges[i].e][newOri];
		}
	} 

	return FaceletCube(facelets);
}

/*
** TO UNDERSTAND HOW COORDINATES ARE COMPUTED, SEE:
** http://kociemba.org/math/coordlevel.htm
** http://kociemba.org/math/twophase.htm
*/

u_int16_t
CubieCube::getCornerOriCoord() const
{
	int coord	= 0;
	int weight	= 0;

	for (int i = CORNER_COUNT - 2; i >= 0; --i)
	{
		coord += _corners[i].o * pow(3, weight);
		weight++;
	}
	return coord;
}

u_int16_t
CubieCube::getEdgeOriCoord() const
{
	int coord	= 0;
	int weight	= 0;

	for (int i = EDGE_COUNT - 2; i >= 0; --i)
	{
		coord += _edges[i].o * pow(2, weight);
		weight++;
	}
	return coord;
}

u_int16_t
CubieCube::getCornerPermCoord() const
{
	int coord = 0;

	for (int i = 1; i < CORNER_COUNT; i++)
	{
		int sum = 0;

		for (int j = 0; j < i; j++)
		{
			if (_corners[j].c > _corners[i].c)
				sum++;
		}
		coord += sum * factorial(i);
	}
	return coord;
}

u_int16_t
CubieCube::getUDSliceCoord() const
{
	int	coord	= 0;
	int k		= 0;
	int	n		= 0;

	while (_edges[n].e < FR) // an edge less than FR is not a UD-slice edge
		n++;

	for (n = n + 1; n < EDGE_COUNT; n++)
	{
		if (_edges[n].e >= FR) // the edge is a UD-slice edge
			k++;
		else
			coord += binomial(n, k);
	}

	return coord;
}

u_int16_t
CubieCube::getPhase2EdgePermCoord() const
{
	int coord = 0;

	for (int i = DB; i > UR; --i)
	{
		int sum = 0;

		for (int j = i - 1; j >= UR; --j)
		{
			if (_edges[j].e > _edges[i].e)
				sum++;
		}
		coord += sum * factorial(i);
	}
	return coord;
}

u_int16_t
CubieCube::getPhase2UDSliceCoord() const
{
	Edge	UDSliceEdges[4];
	int		j = 0;

	for (int i = UR; i <= BR; i++)
	{
		if (_edges[i].e >= FR) // edge[i] is a UDSliceEdge
		{
			UDSliceEdges[j] = _edges[i].e;
			j++;
		}
	}

	int coord = 0;
	for (j = 3; j > 0; --j)
	{
		int	sum = 0;
		for (int k = j - 1; k >= 0; --k)
		{
			if (UDSliceEdges[k] > UDSliceEdges[j])
				sum++;
		}
		coord = (coord + sum) * j;
	}
	return coord;
}

void
CubieCube::setCornerOriCoord(u_int16_t coordinate)
{
	if (coordinate > CORN_ORI_MAX)
		throw std::out_of_range(std::to_string(coordinate) + " is not a valid coordinate value");

	int orientationSum = 0;

	// convert coordinate using ternary number system
	for (int i = CORNER_COUNT - 2; i >= 0; --i)
	{
		int mod = coordinate % 3; // extract the rightest digit

		_corners[i].o = mod; // set corner orientation
		orientationSum += mod;
		coordinate /= 3;
	}

	// last corner is determined by the others
	// the sum of all orientations must be divisible by 3
	_corners[CORNER_COUNT - 1].o = 3 - orientationSum % 3;
}

void
CubieCube::setEdgeOriCoord(u_int16_t coordinate)
{
	if (coordinate > EDGE_ORI_MAX)
		throw std::out_of_range(std::to_string(coordinate) + " is not a valid coordinate value");

	int orientationSum = 0;

	// same as for the corners but with binary system
	for (int i = EDGE_COUNT - 2; i >= 0; --i)
	{
		int mod = coordinate % 2;

		_edges[i].o = mod;
		orientationSum += mod;
		coordinate /= 2;
	}
	_edges[EDGE_COUNT - 1].o = orientationSum % 2;
}

void
CubieCube::setCornerPermCoord(u_int16_t coordinate)
{
	if (coordinate > CORN_PERM_MAX)
		throw std::out_of_range(std::to_string(coordinate) + " is not a valid coordinate value");

	std::vector<Corner>	remainingCorners({ URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB });
	int					score[CORNER_COUNT];

	// loop through the corners in descending order
	for (int i = CORNER_COUNT - 1; i >= 0; --i)
	{
		const int f = factorial(i);

		score[i] = coordinate / f; // retrieve corner's "score"
		coordinate -= score[i] * f;

		// find the corner whose value is strictly inferior than "score[i]" other corners
		const int index = remainingCorners.size() - score[i] - 1;

		_corners[i].c = remainingCorners[index]; // place the corner
		remainingCorners.erase(remainingCorners.begin() + index); // one corner less to place now
	}
}

void		
CubieCube::setUDSliceCoord(u_int16_t coordinate)
{
	if (coordinate > UD_SLICE_MAX)
		throw std::out_of_range(std::to_string(coordinate) + " is not a valid coordinate value");

	int	n = EDGE_COUNT - 1;
	int	k = 3; // k+1 is the number of UD-slice edges left to place

	// reverse the computation
	while (coordinate != 0)
	{
		int res = binomial(n, k);

		if (coordinate >= res) // not a UDSlice edge
		{
			_edges[n].e = UR; // place a random edge
			coordinate -= res;
		}
		else // UD-slice edge found !
		{
			_edges[n].e = FR; // place UD-slice edge
			k--;
		}
		n--;
	}

	// place the last edges
	while (n >= 0)
	{
		if (k >= 0) // remaining UD-slice edges
		{
			_edges[n].e = FR;
			k--;
		}
		else
			_edges[n].e = UR;
		n--;
	}
}

void
CubieCube::setPhase2EdgePermCoord(u_int16_t coordinate)
{
	if (coordinate > EDGE_P2_PERM_MAX)
		throw std::out_of_range(std::to_string(coordinate) + " is not a valid coordinate value");

	std::vector<Edge>	remainingEdges({ UR, UF, UL, UB, DR, DF, DL, DB });
	int					score[EDGE_COUNT - 4];

	// we only place the first 8 edges this time
	for (int i = DB; i >= UR; --i)
	{
		const int f = factorial(i);

		score[i] = coordinate / f;
		coordinate -= score[i] * f;

		const int index = remainingEdges.size() - score[i] - 1;

		_edges[i].e = remainingEdges[index];
		remainingEdges.erase(remainingEdges.begin() + index);
	}
}

void
CubieCube::setPhase2UDSliceCoord(u_int16_t coordinate)
{
	if (coordinate > UD_SLICE_P2_MAX)
		throw std::out_of_range(std::to_string(coordinate) + " is not a valid coordinate value");

	std::vector<Edge>	remainingEdges({ FR, FL, BL, BR });

	for (int i = BR; i > FR; --i)
	{
		const u_int f = factorial(i - 8);

		const int score = coordinate / f;
		coordinate -= score * f;

		const int index = remainingEdges.size() - score - 1;

		_edges[i].e = remainingEdges[index];
		remainingEdges.erase(remainingEdges.begin() + index);
	}
	_edges[FR].e = remainingEdges.front();
}
