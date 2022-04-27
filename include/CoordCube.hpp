/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 01:24:53 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/27 03:45:30 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORDCUBE_HPP
# define CORDCUBE_HPP

# include "MoveTables.hpp"

class CoordCube : public ACube
{
	using CubeState = struct CubeState {
		int		corners;
		int		edges;
		int		UDSlice;
		Move	last;

		bool operator==(const CubeState& rhs);
	};

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
		std::list<std::string>	solvePhase1();
		std::list<std::string>	solvePhase2();

	private:

		void					_generatePruningTables(std::vector<int>& table, int& coord);

		int						_estimateCost(const CubeState& st);
		bool					_isGoalState(const CubeState& st);
		int						_search(std::list<CubeState>& path, int cost, int threshold);
		std::list<CubeState>	_applyAllMoves(const CubeState& node);

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

#endif
