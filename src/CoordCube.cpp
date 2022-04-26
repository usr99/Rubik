/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 01:11:36 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/26 23:07:34 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "../include/CoordCube.hpp"

CoordCube::CoordCube(const std::list<std::string>& sequence) :
	_cornersPruning(CORN_ORI_MAX + 1, -1),
	_edgesPruning(EDGE_ORI_MAX + 1, -1),
	_UDSlicePruning(UD_SLICE_MAX + 1, -1)
{
	setSolvedState();
	_generatePruningTables(_UDSlicePruning, _UDSlice);
	_generatePruningTables(_cornersPruning, _cornersOri);
	_generatePruningTables(_edgesPruning, _edgesOri);

	setSolvedState();
	this->scramble(sequence);
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

bool
CoordCube::CubeState::operator==(const CubeState& rhs)
{
	return (
		rhs.corners == corners &&
		rhs.edges == edges &&
		rhs.UDSlice == UDSlice
	); // we ignore the move which produced this state
}

void
CoordCube::setSolvedState()
{
	_cornersOri	= 0;
	_edgesOri	= 0;
	_UDSlice	= 0;
}

void
CoordCube::move(char face, int factor)
{
	int		faceIndex = 0;
	u_int	moveIndex;

	while (Rubik::Faces[faceIndex] != face)
		faceIndex++;
	moveIndex = faceIndex * 3 + factor - 1;
	if (moveIndex >= MOVES_COUNT)
		throw std::invalid_argument("Accepted moves are U, R, F, D, L and B");

	// find new coordinates in move tables
	_cornersOri	= _moves.tables[MoveTables::CORNER_ORI][_cornersOri][moveIndex];
	_edgesOri	= _moves.tables[MoveTables::EDGE_ORI][_edgesOri][moveIndex];
	_UDSlice	= _moves.tables[MoveTables::UD_SLICE][_UDSlice][moveIndex];
}

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
		// search any solution of length threshold
		const int minimalCost = _search(path, 0, threshold);
		threshold = minimalCost; // new threshold is the smallest branch cost encountered
		found = !minimalCost; // but 0 is returned when the solution has been found
	}

	// convert the path into a readable solution
	path.pop_front(); // remove root node
	std::list<std::string> solution;
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
	}
	return solution;
}

void
CoordCube::_generatePruningTables(std::vector<int>& table, int& coord)
{
	std::list<int>	buffer(1, 0);
	std::list<int>	tmp;
	size_t				filled	= 0;
	size_t				depth	= 0;

	while (filled < table.size())
	{
		for (
			std::list<int>::iterator it = buffer.begin();
			it != buffer.end();
			it++
		) {
			if (table[*it] == -1)
			{
				table[*it] = depth;
				filled++;
			}

			for (int i = 0; i < MOVES_COUNT; i++)
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

int
CoordCube::_search(std::list<CubeState>& path, int cost, int threshold)
{
	const CubeState&	node		= path.back();
	const int			branchCost	= cost + _estimateCost(node);
	int					minimalCost	= -1;

	if (branchCost > threshold)
		return branchCost; // this branch is a dead-end
	if (_isGoalState(node))
		return 0; // solution found

	std::list<CubeState> nextNodes = _applyAllMoves(node);
	for (
		std::list<CubeState>::const_iterator it = nextNodes.begin();
		it != nextNodes.end();
		it++
	) {
		if (std::find(path.begin(), path.end(), *it) == path.end()) // avoid going back to a previously explored state
		{
			path.push_back(*it);

			const int c = _search(path, cost + 1, threshold);
			if (c == 0)
				return c; // solution found
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
	return ( // biggest value from pruning tables
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
	std::list<CubeState> results;

	for (int i = 0; i < MOVES_COUNT; i++) // apply all possible moves
	{
		// except those who would reverse the current state
		// i.e. moves on the same face as the last one
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
	return results; // list of the states resulting from applying a move on the current node
}
