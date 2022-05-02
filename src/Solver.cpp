/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:34:59 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 15:39:02 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Solver.hpp"

template <>
int Solver<CubeStateP1>::_estimateCost(const CubeStateP1& st)
{
	return ( // biggest value from pruning tables
		std::max(
			(*_prun)[0][st.c][st.e],
			(*_prun)[1][st.e][st.ud]
		)
	);
}

template <>
int
Solver<CubeStateP2>::_estimateCost(const CubeStateP2& st)
{
	return ( // biggest value from pruning tables
		std::max(
			(*_prun)[2][st.c][st.ud],
			(*_prun)[3][st.e][st.ud]
		)
	);
}

CubeState::CubeState(int c, int e, int ud, Move last) :
	c(c), e(e), ud(ud), last(last) {}

bool
CubeState::isGoal() const
{
	// all coordinates must be zero
	return (!c && !e && !ud);
}

bool
CubeState::operator==(const CubeState& rhs) const
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
CubeStateP1::AllowedMoves = Rubik::GroupP1;

const std::vector<Move>&
CubeStateP2::AllowedMoves = Rubik::GroupP2;

CubeStateP1::CubeStateP1(const MoveTables* tables, const CubeStateP1& st, Move m) :
	CubeState(
		(*tables)[MoveTables::CORNER_ORI][st.c][m],
		(*tables)[MoveTables::EDGE_ORI][st.e][m],
		(*tables)[MoveTables::UD_SLICE][st.ud][m],
		m
	) {}

CubeStateP1::CubeStateP1(const CubieCube& cube) :
	CubeState(
		cube.getCornerOriCoord(),
		cube.getEdgeOriCoord(),
		cube.getUDSliceCoord()
	) {}

CubeStateP2::CubeStateP2(const MoveTables* tables, const CubeStateP2& st, Move m) :
	CubeState(
		(*tables)[MoveTables::CORNER_PERM][st.c][m],
		(*tables)[MoveTables::EDGE_P2][st.e][m],
		(*tables)[MoveTables::UD_SLICE_P2][st.ud][m],
		m
	) {}

CubeStateP2::CubeStateP2(const CubieCube& cube) :
	CubeState(
		cube.getCornerPermCoord(),
		cube.getPhase2EdgePermCoord(),
		cube.getPhase2UDSliceCoord()
	) {}
