/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubieCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:29:37 by user42            #+#    #+#             */
/*   Updated: 2022/04/26 19:22:29 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBIECUBE_HPP
# define CUBIECUBE_HPP

# include "FaceletCube.hpp"
# include "cubie.hpp"

class CubieCube : public ACube
{
    public:

		// constructor
        CubieCube(const std::list<std::string>& sequence = {});	// default
        CubieCube(const CubieCube& rhs);						// copy

		// destructor
		~CubieCube();

		// operators
		CubieCube&		operator=(const CubieCube& rhs);
		bool			operator==(const CubieCube& rhs);

		// member functions
		virtual void	move(char face, int factor = 1);
		FaceletCube		toFacelet();

		// coordinates getters
		int		getCornerOriCoord() const;
		int		getEdgeOriCoord() const;
		int		getCornerPermCoord() const;
		int		getEdgePermCoord() const;
		int		getUDSliceCoord() const;
		int		getPhase2EdgePermCoord() const;
		int		getPhase2UDSliceCoord() const;
		// coordinates setters
		void	setCornerOriCoord(int coordinate);
		void	setEdgeOriCoord(int coordinate);
		void	setCornerPermCoord(int coordinate);
		void	setEdgePermCoord(int coordinate);
		void	setUDSliceCoord(int coordinate);
		void	setPhase2EdgePermCoord(int coordinate);
		void	setPhase2UDSliceCoord(int coordinate);
		
    private:

		CornerCubie	_corners[CORNER_COUNT];
		EdgeCubie	_edges[EDGE_COUNT];
};

#endif
