/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 01:24:53 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/29 23:17:01 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COORDCUBE_HPP
# define COORDCUBE_HPP

# include "PruningTables.hpp"

class CoordCube : public ACube
{
	public:

		// constructor
		CoordCube(const std::list<std::string>& sequence = {});	// default
		CoordCube(const CoordCube& rhs);						// copy
		CoordCube(const CubieCube& rhs);						// from cubie level

		// destructor
		~CoordCube();

		// operators
		CoordCube&				operator=(const CoordCube& rhs);
		CoordCube&				operator=(const CubieCube& rhs);

		// member functions
		void					setSolvedState();
		virtual void			move(char face, int factor = 1);
		std::list<std::string>	solve();

	private:

		// nested structures used in the solver
		struct CubeState;
		struct CubeStateP1;
		struct CubeStateP2;

		template <typename T>
		std::list<std::string>	_solve();
		template <typename T>
		int						_estimateCost(const T& st);
		template <typename T>
		int						_search(std::list<T>& path, int cost, int threshold);
		template <typename T>
		std::list<T>			_applyAllMoves(const T& node);

		int						_cornersOri;
		int						_edgesOri;
		int						_UDSlice;
		int						_cornersPerm;
		int						_edgesPermP2;
		int						_UDSliceP2;

		const MoveTables&		_moves;
		const PruningTables&	_pruning;

		// TO REMOVE AS SOON AS POSSIBLE
		const std::list<std::string>	_scramble;
};

struct CoordCube::CubeState
{
	CubeState(int c, int e, int ud, Move last = NONE);

	bool isGoal() const;
	bool operator==(const CubeState& rhs) const;

	int		c;
	int		e;
	int		ud;
	Move	last;
};

struct CoordCube::CubeStateP1 : public CoordCube::CubeState
{
	CubeStateP1(const CoordCube& cube);
	CubeStateP1(int c, int e, int ud, Move last = NONE);

	static const std::vector<Move>& AllowedMoves;
};

struct CoordCube::CubeStateP2 : public CoordCube::CubeState
{
	CubeStateP2(const CoordCube& cube);
	CubeStateP2(int c, int e, int ud, Move last = NONE);

	static const std::vector<Move>& AllowedMoves;
};

# include "solver.ipp"

#endif
