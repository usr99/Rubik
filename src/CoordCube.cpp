/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 01:11:36 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/27 22:11:39 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "../include/CoordCube.hpp"

CoordCube::CoordCube(const std::list<std::string>& sequence) :
	_cornersPruning(CORN_ORI_MAX + 1, -1),
	_edgesPruning(EDGE_ORI_MAX + 1, -1),
	_UDSlicePruning(UD_SLICE_MAX + 1, -1),
	_edgesP2Pruning(EDGE_P2_PERM_MAX + 1, -1),
	_UDSliceP2Pruning(UD_SLICE_P2_MAX + 1, -1)
{
	setSolvedState();
	std::cout << "Generating pruning tables ...\n";
	_generatePruningTables(_UDSlicePruning, _UDSlice);
	_generatePruningTables(_cornersPruning, _cornersOri);
	_generatePruningTables(_edgesPruning, _edgesOri);
	_generatePruningTables(_edgesP2Pruning, _edgesPermP2);
	_generatePruningTables(_UDSliceP2Pruning, _UDSliceP2);
	std::cout << "done !\n";

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

void
CoordCube::setSolvedState()
{
	_cornersOri		= 0;
	_edgesOri		= 0;
	_UDSlice		= 0;
	_edgesPermP2	= 0;
	_UDSliceP2		= 0;
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
	_cornersOri		= _moves.tables[MoveTables::CORNER_ORI][_cornersOri][moveIndex];
	_edgesOri		= _moves.tables[MoveTables::EDGE_ORI][_edgesOri][moveIndex];
	_UDSlice		= _moves.tables[MoveTables::UD_SLICE][_UDSlice][moveIndex];
	_edgesPermP2	= _moves.tables[MoveTables::EDGE_P2][_edgesPermP2][moveIndex];
	_UDSliceP2		= _moves.tables[MoveTables::UD_SLICE_P2][_UDSliceP2][moveIndex];
}

std::list<std::string>
CoordCube::solve()
{
	std::list<std::string> solution;
	std::list<std::string> tmp;

	solution = _solve<CubeStateP1>();
	std::cout << "phase 1 solved\n";
	//tmp = _solve<CubeStateP2>();
	//std::cout << "phase 2 solved\n";
	//solution.insert(solution.end(), tmp.begin(), tmp.end());
	return solution;
}

void
CoordCube::_generatePruningTables(std::vector<int>& table, int& coord)
{
	std::list<int>	buffer(1, 0);
	std::list<int>	tmp;
	size_t			filled	= 0;
	size_t			depth	= 0;

	while (filled < table.size() / 2)
	{
		for (
			std::list<int>::const_iterator it = buffer.begin();
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
	
	buffer.clear();
	for (size_t i = 1; i < table.size(); i++)
	{
		if (table[i] == -1)
			buffer.push_back(i);
	}

	while (buffer.size())
	{
		std::list<int>::const_iterator it = buffer.begin();
		
		while (it != buffer.end())
		{
			bool found = false;

			for (int i = 0; !found && i < MOVES_COUNT; i++)
			{
				if (i % 3 == 0)
					coord = *it;

				move(Rubik::Faces[i / 3]);

				if (table[coord] >= 0)
				{
					found = true;
					table[*it] = table[coord] + 1;
					it = buffer.erase(it);
					filled++;
				}
			}

			if (!found)
				it++;
		}
	}
}

template <>
int CoordCube::_estimateCost(const CubeStateP1& st)
{
	return ( // biggest value from pruning tables
		std::max(
			_cornersPruning[st.c],
			std::max(
				_edgesPruning[st.e],
				_UDSlicePruning[st.ud]
			)
		)
	);
}

template <>
int
CoordCube::_estimateCost(const CubeStateP2& st)
{
	return ( // biggest value from pruning tables
		std::max(
			_edgesP2Pruning[st.e],
			_UDSliceP2Pruning[st.ud]
		)
	);
}

template <>
std::list<CoordCube::CubeStateP1>
CoordCube::_applyAllMoves(const CubeStateP1& node)
{
	std::list<CubeStateP1> results;

	for (int i = 0; i < MOVES_COUNT; i++) // apply all possible moves
	{
		// except those who would reverse the current state or commutating ones
		// i.e. moves on the same/opposite face as the last one
		const int currentMoveIndex	= i / 3;
		const int lastMoveIndex		= node.last / 3;

		if (
			currentMoveIndex != lastMoveIndex &&
			currentMoveIndex != (lastMoveIndex + 3) % FACES_COUNT
		) {
			results.push_back(CubeStateP1(
				_moves.tables[MoveTables::CORNER_ORI][node.c][i],
				_moves.tables[MoveTables::EDGE_ORI][node.e][i],
				_moves.tables[MoveTables::UD_SLICE][node.ud][i],
				static_cast<Move>(i)
			));
		}
	}
	return results; // list of the states resulting from applying a move on the current node
}

template <>
std::list<CoordCube::CubeStateP2>
CoordCube::_applyAllMoves(const CoordCube::CubeStateP2& node)
{
	std::list<CubeStateP2>	results;
	std::list<Move>			availableMoves({
		Ux1, Ux2, Ux3,
		Dx1, Dx2, Dx3,
		Rx2, Fx2, Lx2, Bx2
	});

	for (
		std::list<Move>::const_iterator it = availableMoves.begin();
		it != availableMoves.end();
		it++
	) { // apply all available moves
		// except those who would reverse the current state or commutating ones
		// i.e. moves on the same/opposite face as the last one
		const int currentMoveIndex	= *it / 3;
		const int lastMoveIndex		= node.last / 3;

		if (
			currentMoveIndex != lastMoveIndex &&
			currentMoveIndex != (lastMoveIndex + 3) % FACES_COUNT
		) {
			results.push_back(CubeStateP2(
				_moves.tables[MoveTables::EDGE_P2][node.e][*it],
				_moves.tables[MoveTables::UD_SLICE_P2][node.ud][*it],
				*it
			));
		}
	}
	return results; // list of the states resulting from applying a move on the current node
}

CoordCube::ACubeState::ACubeState(Move last) : last(last) {}

CoordCube::CubeStateP1::CubeStateP1(const CoordCube& cube) :
	c(cube._cornersOri), e(cube._edgesOri), ud(cube._UDSlice) {}

CoordCube::CubeStateP1::CubeStateP1(int c, int e, int ud, Move last) : ACubeState(last),
	c(c), e(e), ud(ud) {}

bool
CoordCube::CubeStateP1::isGoal() const
{
	// all coordinates must be zero
	return (!c && !e && !ud);
}

bool
CoordCube::CubeStateP1::operator==(const CubeStateP1& rhs) const
{
	// 2 states are equals if and only if their coordinates are equals
	// last move is ignored
	return (
		c == rhs.c &&
		e == rhs.e &&
		ud == rhs.ud
	);
}

CoordCube::CubeStateP2::CubeStateP2(const CoordCube& cube) :
	e(cube._edgesPermP2), ud(cube._UDSliceP2) {}

CoordCube::CubeStateP2::CubeStateP2(int e, int ud, Move last) : ACubeState(last),
	e(e), ud(ud) {}

bool
CoordCube::CubeStateP2::isGoal() const
{
	return (!e && !ud);
}

bool
CoordCube::CubeStateP2::operator==(const CubeStateP2& rhs) const
{
	return (
		e == rhs.e &&
		ud == rhs.ud
	);
}
