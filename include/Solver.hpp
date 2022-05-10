/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/02 15:08:05 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/10 15:28:08 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_HPP
# define SOLVER_HPP

# include "PruningTables.hpp"

template <typename T>
class Solver
{
	public:

		Solver();
		
		std::list<std::string>	solve(const T& root);

	private:

		int						_estimateCost(const T& st);
		int						_search(std::list<T>& path, int cost, int threshold);
		std::list<T>			_applyAllMoves(const T& node);

		const MoveTables*		_mt;
		const PruningTables*	_prun;
};

struct CubeState
{
	CubeState(int c, int e, int ud, Move last = NONE);

	bool isGoal() const;
	bool operator==(const CubeState& rhs) const;

	int		c;
	int		e;
	int		ud;
	Move	last;
};

struct CubeStateP1 : public CubeState
{
	CubeStateP1(const CubieCube& cube);
	CubeStateP1(const MoveTables* tables, const CubeStateP1& st, Move m);

	static const std::vector<Move>& AllowedMoves;
};

struct CubeStateP2 : public CubeState
{
	CubeStateP2(const CubieCube& cube);
	CubeStateP2(const MoveTables* tables, const CubeStateP2& st, Move m);

	static const std::vector<Move>& AllowedMoves;
};

# include "Solver.ipp"

std::list<std::string>	solve(const CubieCube& cube);
std::list<std::string>	solve(const std::list<std::string>& sequence);

#endif
