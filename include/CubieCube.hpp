/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubieCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:29:37 by user42            #+#    #+#             */
/*   Updated: 2022/05/02 01:06:13 by mamartin         ###   ########.fr       */
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
		u_int16_t	getCornerOriCoord() const;
		u_int16_t	getEdgeOriCoord() const;
		u_int16_t	getCornerPermCoord() const;
		u_int16_t	getUDSliceCoord() const;
		u_int16_t	getPhase2EdgePermCoord() const;
		u_int16_t	getPhase2UDSliceCoord() const;
		// coordinates setters
		void		setCornerOriCoord(u_int16_t coordinate);
		void		setEdgeOriCoord(u_int16_t coordinate);
		void		setCornerPermCoord(u_int16_t coordinate);
		void		setUDSliceCoord(u_int16_t coordinate);
		void		setPhase2EdgePermCoord(u_int16_t coordinate);
		void		setPhase2UDSliceCoord(u_int16_t coordinate);
		
    private:

		CornerCubie	_corners[CORNER_COUNT];
		EdgeCubie	_edges[EDGE_COUNT];
};

#endif
