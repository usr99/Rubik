/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CoordCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/24 01:24:53 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/25 23:32:06 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORDCUBE_HPP
# define CORDCUBE_HPP

# include "MoveTables.hpp"

class CoordCube : public ACube
{
	using CubeState = struct CubeState {
		long corners;
		long edges;
		long UDSlice;
		Move last;

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
		virtual void			move(char face, int factor = 1);
		std::list<std::string>	solvePhase1();

	private:

		void					_generatePruningTables(std::vector<long>& table, long& coord);

		int						_estimateCost(const CubeState& st);
		bool					_isGoalState(const CubeState& st);
		int						_search(std::list<CubeState>& path, int cost, int threshold);
		std::list<CubeState>	_applyAllMoves(const CubeState& node);

		long					_cornersOri;
		long					_edgesOri;
		long					_UDSlice;

		std::vector<long>		_cornersPruning;
		std::vector<long>		_edgesPruning;
		std::vector<long>		_UDSlicePruning;
		
		const MoveTables		_moves;
};

#endif
