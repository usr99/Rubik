/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubieCube.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:52:27 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 23:32:03 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CubieCube.hpp"
#include "../include/Move.hpp"

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
	const std::vector<CornerCubie>&	cTable(Move::cornerTable.at(face));
	const std::vector<EdgeCubie>&	eTable(Move::edgeTable.at(face));
	const CubieCube					copy(*this);

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
}
