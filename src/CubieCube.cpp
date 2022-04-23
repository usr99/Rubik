/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubieCube.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:52:27 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 22:20:59 by mamartin         ###   ########.fr       */
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

void
CubieCube::move(char face, int factor)
{
	try {
		const std::vector<CornerCubie>&	cTable(Move::cornerTable.at(face));
		const std::vector<EdgeCubie>&	eTable(Move::edgeTable.at(face));

		const CubieCube copy(*this);

		for (int j = 0; j < factor; j++)
		{
			for (int i = 0; i < CORNER_COUNT; i++)
			{
				_corners[i].c = copy._corners[cTable[i].c].c;
				_corners[i].o = (_corners[i].o + cTable[i].o) % 3;
			}
			for (int i = 0; i < EDGE_COUNT; i++)
			{
				_edges[i].e = copy._edges[eTable[i].e].e;
				_edges[i].o = (_edges[i].o + eTable[i].o) % 2;
			}
		}
	} catch (const std::out_of_range& e) {
		throw std::invalid_argument(std::string("Accepted moves are U, R, F, D, L and B"));
	}
}

long
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

long
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

long
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
		coord += sum * factorial(i + 1);
	}
	return coord;
}

long
CubieCube::getEdgePermCoord() const
{
	int coord = 0;

	for (int i = 1; i < EDGE_COUNT; i++)
	{
		int sum = 0;

		for (int j = 0; j < i; j++)
		{
			if (_edges[j].e > _edges[i].e)
				sum++;
		}
		coord += sum * factorial(i + 1);
	}
	return coord;
}

long
CubieCube::getUDSliceCoord() const
{
	int	coord	= 0;
	int k		= 0;
	int	n		= 0;

	while (_edges[n].e < FR)
		n++;

	for (n = n + 1; n < EDGE_COUNT; n++)
	{
		if (_edges[n].e >= FR)
			k++;
		else
			coord += binomial(n, k);
	}

	return coord;
}

void
CubieCube::setCornerOriCoord(long coordinate)
{
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
	_corners[CORNER_COUNT - 1].o = orientationSum % 3;
}

void
CubieCube::setEdgeOriCoord(long coordinate)
{
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
CubieCube::setCornerPermCoord(long coordinate)
{
	std::vector<Corner>	remainingCorners({ URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB });
	int					orders[CORNER_COUNT];

	for (int i = CORNER_COUNT - 1; i >= 0; --i)
	{
		const int f = factorial(i + 1);

		orders[i] = coordinate / f;	// compute corner's "score"
		coordinate -= orders[i] * f;

		const int index = remainingCorners.size() - orders[i] - 1;

		_corners[i].c = remainingCorners[index]; // set the corner
		remainingCorners.erase(remainingCorners.begin() + index); // one corner less to place
	}
}

void
CubieCube::setEdgePermCoord(long coordinate)
{
	std::vector<Edge>	remainingEdges({ UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR });
	int					orders[EDGE_COUNT];

	for (int i = EDGE_COUNT - 1; i >= 0; --i)
	{
		const int f = factorial(i + 1);

		orders[i] = coordinate / f;
		coordinate -= orders[i] * f;

		const int index = remainingEdges.size() - orders[i] - 1;

		_edges[i].e = remainingEdges[index];
		remainingEdges.erase(remainingEdges.begin() + index);
	}
}

void		
CubieCube::setUDSliceCoord(long coordinate)
{
	int	n = EDGE_COUNT - 1;
	int	k = 3;

	while (coordinate != 0)
	{
		int res = binomial(n, k);

		if (coordinate >= res)
		{
			// not a UDSlice edge
			_edges[n].e = UR;
			coordinate -= res;
		}
		else
		{
			// UDSlice edge found !
			_edges[n].e = FR;
			k--;
		}
		n--;
	}

	while (n >= 0)
	{
		if (k >= 0)
		{
			_edges[n].e = FR; // last UDSlice edges
			k--;
		}
		else 
			_edges[n].e = UR;
		n--;
	}
}
