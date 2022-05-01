/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/25 01:11:36 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/01 01:15:38 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CoordCube.hpp"

CoordCube::CoordCube(const std::list<std::string>& sequence) :
	_moves(*MoveTables::getInstance()), _pruning(*PruningTables::getInstance(_moves)), _scramble(sequence)
{
	setSolvedState();
	this->scramble(sequence);
}

CoordCube::CoordCube(const CoordCube& rhs) : CoordCube()
{
	*this = rhs;
}

CoordCube::CoordCube(const CubieCube& rhs) : CoordCube()
{
	*this = rhs;
}

CoordCube::~CoordCube()
{
	delete &_moves;
	delete &_pruning;
}

CoordCube&
CoordCube::operator=(const CoordCube& rhs)
{
	_cornersOri		= rhs._cornersOri;
	_edgesOri		= rhs._edgesOri;
	_UDSlice		= rhs._UDSlice;
	_cornersPerm	= rhs._cornersPerm;
	_edgesPermP2	= rhs._edgesPermP2;
	_UDSliceP2		= rhs._UDSliceP2;
	return *this;
}

CoordCube&
CoordCube::operator=(const CubieCube& rhs)
{
	_cornersOri		= rhs.getCornerOriCoord();
	_edgesOri		= rhs.getEdgeOriCoord();
	_UDSlice		= rhs.getUDSliceCoord();
	_cornersPerm	= rhs.getCornerPermCoord();
	_edgesPermP2	= rhs.getPhase2EdgePermCoord();
	_UDSliceP2		= rhs.getPhase2UDSliceCoord();
	return *this;
}

void
CoordCube::setSolvedState()
{
	_cornersOri		= 0;
	_edgesOri		= 0;
	_UDSlice		= 0;
	_cornersPerm	= 0;
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
	_cornersOri		= _moves[Table::CORNER_ORI][_cornersOri][moveIndex];
	_edgesOri		= _moves[Table::EDGE_ORI][_edgesOri][moveIndex];
	_UDSlice		= _moves[Table::UD_SLICE][_UDSlice][moveIndex];
	_cornersPerm	= _moves[Table::CORNER_PERM][_cornersPerm][moveIndex];
	_edgesPermP2	= _moves[Table::EDGE_P2][_edgesPermP2][moveIndex];
	_UDSliceP2		= _moves[Table::UD_SLICE_P2][_UDSliceP2][moveIndex];
}

std::list<std::string>
CoordCube::solve()
{
	std::list<std::string> solution;
	std::list<std::string> tmp;

	// Phase 1
	solution = _solve<CubeStateP1>();
	
	// Apply the Phase 1 solution
	CubieCube c(_scramble);
	c.scramble(solution);
	*this = c;

	// Phase 2
	tmp = _solve<CubeStateP2>();

	// return solution
	solution.insert(solution.end(), tmp.begin(), tmp.end());
	return solution;
}

template <>
int CoordCube::_estimateCost(const CubeStateP1& st)
{
	return ( // biggest value from pruning tables
		std::max(
			_pruning[0][st.c][st.e],
			_pruning[1][st.e][st.ud]
		)
	);
}

template <>
int
CoordCube::_estimateCost(const CubeStateP2& st)
{
	return ( // biggest value from pruning tables
		std::max(
			_pruning[2][st.c][st.ud],
			_pruning[3][st.e][st.ud]
		)
	);
}

CoordCube::CubeState::CubeState(int c, int e, int ud, Move last) :
	c(c), e(e), ud(ud), last(last) {}

bool
CoordCube::CubeState::isGoal() const
{
	// all coordinates must be zero
	return (!c && !e && !ud);
}

bool
CoordCube::CubeState::operator==(const CubeState& rhs) const
{
	// 2 states are equals if and only if their coordinates are equals
	// last move is ignored
	return (
		c == rhs.c &&
		e == rhs.e &&
		ud == rhs.ud
	);
}

const std::vector<Move>&
CoordCube::CubeStateP1::AllowedMoves = Rubik::GroupP1;

const std::vector<Move>&
CoordCube::CubeStateP2::AllowedMoves = Rubik::GroupP2;

CoordCube::CubeStateP1::CubeStateP1(const MoveTables& tables, const CubeStateP1& st, Move m) :
	CubeState(
		tables[Table::CORNER_ORI][st.c][m],
		tables[Table::EDGE_ORI][st.e][m],
		tables[Table::UD_SLICE][st.ud][m],
		m
	) {}

CoordCube::CubeStateP1::CubeStateP1(const CoordCube& cube) :
	CubeState(cube._cornersOri, cube._edgesOri, cube._UDSlice) {}

CoordCube::CubeStateP2::CubeStateP2(const MoveTables& tables, const CubeStateP2& st, Move m) :
	CubeState(
		tables[Table::CORNER_PERM][st.c][m],
		tables[Table::EDGE_P2][st.e][m],
		tables[Table::UD_SLICE_P2][st.ud][m],
		m
	) {}

CoordCube::CubeStateP2::CubeStateP2(const CoordCube& cube) :
	CubeState(cube._cornersPerm, cube._edgesPermP2, cube._UDSliceP2) {}
