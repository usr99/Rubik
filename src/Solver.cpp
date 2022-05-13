/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:34:59 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/13 18:12:22 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctime>
#include <random>
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

std::list<std::string> solve(const CubieCube& cube)
{
	// Solve phase 1
	Solver<CubeStateP1>		solverP1;
	std::list<std::string>	solution = solverP1.solve(CubeStateP1(cube));

	// Apply the solution found to a copy of the cube
	CubieCube copy(cube);
	copy.scramble(solution);

	// Solve phase 2 from the new state
	Solver<CubeStateP2>		solverP2;
	std::list<std::string>	tmp = solverP2.solve(CubeStateP2(copy));

	if (!solution.size())
		return solution;

	// Modify the moves transitioning between the two phases if they're on the same face
	if (solution.back().front() == tmp.front().front())
	{
		const std::vector<char>	values	= { '\0', '2', '\'' };
		char						v1		= 1;
		char						v2		= 1;
		char						move	= solution.back().front();

		if (solution.back().size() > 1)
			v1 = (std::find(values.begin(), values.end(), solution.back()[1]) - values.begin()) + 1;
		if (tmp.front().size() > 1)
			v2 = (std::find(values.begin(), values.end(), tmp.front()[1]) - values.begin()) + 1;

		// erase the moves from the solution
		solution.erase(--solution.end());
		tmp.erase(tmp.begin());

		v1 = (v1 + v2) % 4;
		if (v1) // the moves do not cancel each other, they add up
			solution.push_back(std::string(1, move) + values[v1 - 1]);
	}
	solution.insert(solution.end(), tmp.begin(), tmp.end());

	return solution;
}

std::list<std::string>	solve(const std::list<std::string>& sequence)
{
	return solve(CubieCube(sequence));
}

std::list<std::string>	parseScramble(char** sequences, int count)
{
	std::list<std::string> scramble;
	for (int i = 0; i < count; i++)
	{
		std::istringstream iss(sequences[i]);
		std::string buf;

		while (std::getline(iss, buf, ' '))
		{
			if (buf.size() != 0)
				scramble.push_back(buf);
		}
	}
	return scramble;
}

std::list<std::string>	generateScramble(int length)
{
	/* Setup random number generator */
	std::seed_seq seed({ std::time(nullptr) });
	std::default_random_engine gen(seed);
	std::uniform_int_distribution<> distr_move(0, 3);
	std::uniform_int_distribution<> distr_modifiers(0, 2);

	/* Initiliaze possible moves and modifiers */
	std::array<bool, 6>	allowedMoves;
	const char			modifiers[] = { '2', '\'' };

	allowedMoves.fill(true);
	std::list<std::string> scramble;
	for (int i = 0; i < length; i++)
	{
		int rand = distr_move(gen) + 1;
		int n = -1;

		/* Push the rand-th allowed move */
		while (rand)
		{
			n++;
			if (allowedMoves[n])
				rand--;
		}
		scramble.push_back(std::string(1, Rubik::Faces[n]));

		/* Randomly add a modifier character to the move */
		int modIndex = distr_modifiers(gen);
		if (modIndex != 2)
			scramble.back().push_back(modifiers[modIndex]);

		/* Prevent the next move to be on the same face or the opposite one */
		allowedMoves.fill(true);
		allowedMoves[n] = false;
		allowedMoves[(n + 3) % 6] = false;
	}
	return scramble;
}
