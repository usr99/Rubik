/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 01:11:36 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/26 02:08:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "../include/CoordCube.hpp"

CoordCube::CoordCube(const std::list<std::string>& sequence) :
	_cornersPruning(CORN_ORI_MAX + 1, -1),
	_edgesPruning(EDGE_ORI_MAX + 1, -1),
	_UDSlicePruning(UD_SLICE_MAX + 1, -1)
{
	CubieCube c;

	*this = c;
	_generatePruningTables(_UDSlicePruning, _UDSlice);
	_generatePruningTables(_cornersPruning, _cornersOri);
	_generatePruningTables(_edgesPruning, _edgesOri);

	c.scramble(sequence);
	*this = c;
}

CoordCube::CoordCube(const CoordCube& rhs)
{
	*this = rhs;
}

CoordCube::CoordCube(const CubieCube& rhs)
{
	*this = rhs;
}

CoordCube::~CoordCube() {}

CoordCube&
CoordCube::operator=(const CoordCube& rhs)
{
	_cornersOri	= rhs._cornersOri;
	_edgesOri	= rhs._edgesOri;
	_UDSlice	= rhs._UDSlice;
	return *this;
}

CoordCube&
CoordCube::operator=(const CubieCube& rhs)
{
	_cornersOri	= rhs.getCornerOriCoord();
	_edgesOri	= rhs.getEdgeOriCoord();
	_UDSlice	= rhs.getUDSliceCoord();
	return *this;
}

void
CoordCube::move(char face, int factor)
{
	int faceIndex = 0;
	int moveIndex;

	while (Rubik::Faces[faceIndex] != face)
		faceIndex++;
	moveIndex = faceIndex * 3 + factor - 1;

	_cornersOri	= _moves.tables[MoveTables::CORNER_ORI][_cornersOri][moveIndex];
	_edgesOri	= _moves.tables[MoveTables::EDGE_ORI][_edgesOri][moveIndex];
	_UDSlice	= _moves.tables[MoveTables::UD_SLICE][_UDSlice][moveIndex];
}

void
CoordCube::_generatePruningTables(std::vector<long>& table, long& coord)
{
	std::list<long>	buffer(1, 0);
	std::list<long>	tmp;
	size_t			filled	= 0;
	size_t			depth	= 0;

	while (filled < table.size())
	{
		for (
			std::list<long>::iterator it = buffer.begin();
			it != buffer.end();
			it++
		) {
			if (table[*it] == -1)
			{
				table[*it] = depth;
				filled++;
			}

			for (int i = 0; i < 18; i++)
			{
				if (i % 3 == 0)
					coord = *it;

				move(Rubik::Faces[i / 3]);

				if (table[coord] == -1)
					tmp.push_back(coord);
			}
		}

		buffer = tmp;
		tmp.clear();
		depth++;
	}
}

/**********************************************/
/************** WORK IN PROGRESS **************/
/**********************************************/

std::list<std::string>
CoordCube::solvePhase1()
{
	CubeState root = {
		.corners	= _cornersOri,
		.edges		= _edgesOri,
		.UDSlice	= _UDSlice,
		.last		= NONE // initial state do not result from any move
	};

	std::list<CubeState>	path;
	int						threshold	= _estimateCost(root);
	bool					found		= false;

	path.push_back(root);
	while (!found)
	{
		const int cost = _search(path, 0, threshold);

		threshold = cost;
		found = !cost; // a cost of 0 means the solution is found
	}

	// convert path into a list of moves (as strings)
	path.pop_front();
	std::list<std::string>	solution;
	for (
		std::list<CubeState>::const_iterator it = path.begin();
		it != path.end();
		it++
	) {
		int			moveIndex	= it->last / 3;
		int			factor		= it->last - moveIndex * 3 + 1;
		std::string	move(1, Rubik::Faces[moveIndex]);

		if (factor == 2)
			move += '2';
		else if (factor == 3)
			move += '\'';
		solution.push_back(move);
		std::cout << move << " ";
	}
	std::cout << "\n";
	return solution;
}

int
CoordCube::_search(std::list<CubeState>& path, int cost, int threshold)
{
	const CubeState&	node		= path.back();
	const int			branchCost	= cost + _estimateCost(node);
	int					minimalCost	= -1;

	if (branchCost > threshold)
		return branchCost;
	if (_isGoalState(node))
		return 0; // null cost

	std::list<CubeState> nextNodes = _applyAllMoves(node);
	for (
		std::list<CubeState>::const_iterator it = nextNodes.begin();
		it != nextNodes.end();
		it++
	) {
		if (std::find(path.begin(), path.end(), *it) == path.end())
		{
			path.push_back(*it);

			const int c = _search(path, cost + 1, threshold);
			if (c == 0)
				return c;
			if (c < minimalCost || minimalCost == -1)
				minimalCost = c;
			path.pop_back();
		}
	}
	return minimalCost;
}

int
CoordCube::_estimateCost(const CubeState& st)
{
	return (
		std::max(
			_cornersPruning[st.corners],
			std::max(
				_edgesPruning[st.edges],
				_UDSlicePruning[st.UDSlice]
			)
		)
	);
}

bool
CoordCube::_isGoalState(const CubeState& st)
{
	if (st.corners || st.edges || st.UDSlice)
		return false; // all coordinates must be zero
	return true;
}

std::list<CoordCube::CubeState>
CoordCube::_applyAllMoves(const CubeState& node)
{
	std::list<CubeState>	results;

	for (int i = 0; i < MOVES_COUNT; i++)
	{
		if (i / 3 != node.last / 3)
		{
			results.push_back({
				.corners	= _moves.tables[MoveTables::CORNER_ORI][node.corners][i],
				.edges		= _moves.tables[MoveTables::EDGE_ORI][node.edges][i],
				.UDSlice	= _moves.tables[MoveTables::UD_SLICE][node.UDSlice][i],
				.last		= static_cast<Move>(i)
			});
		}
	}
	return results;
}

bool
CoordCube::CubeState::operator==(const CubeState& rhs)
{
	return (
		rhs.corners == corners &&
		rhs.edges == edges &&
		rhs.UDSlice == UDSlice
	);
}
