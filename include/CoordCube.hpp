/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 01:24:53 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/27 18:04:10 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COORDCUBE_HPP
# define COORDCUBE_HPP

# include "MoveTables.hpp"

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
		struct ACubeState;
		struct CubeStateP1;
		struct CubeStateP2;

		void					_generatePruningTables(std::vector<int>& table, int& coord);

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
		int						_edgesPermP2;
		int						_UDSliceP2;

		std::vector<int>		_cornersPruning;
		std::vector<int>		_edgesPruning;
		std::vector<int>		_UDSlicePruning;
		std::vector<int>		_edgesP2Pruning;
		std::vector<int>		_UDSliceP2Pruning;
		
		const MoveTables		_moves;
};

struct CoordCube::ACubeState
{
	ACubeState(Move last = NONE);

	virtual bool isGoal() const = 0;

	Move last;
};

struct CoordCube::CubeStateP1 : public CoordCube::ACubeState
{
	CubeStateP1(const CoordCube& cube);
	CubeStateP1(int c, int e, int ud, Move last = NONE);

	virtual bool	isGoal() const;
	bool			operator==(const CubeStateP1& rhs) const;

	int	c;
	int	e;
	int	ud;
};

struct CoordCube::CubeStateP2 : public CoordCube::ACubeState
{
	CubeStateP2(const CoordCube& cube);
	CubeStateP2(int e, int ud, Move last = NONE);

	virtual bool	isGoal() const;
	bool			operator==(const CubeStateP2& rhs) const;

	int	e;
	int	ud;
};

# include "Solver.ipp"

#endif
