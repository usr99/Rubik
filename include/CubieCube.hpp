/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubieCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:29:37 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 22:20:17 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBIECUBE_HPP
# define CUBIECUBE_HPP

# include "ACube.hpp"
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

		// member functions
		virtual void	move(char face, int factor = 1);
		// coordinates getters
		long	getCornerOriCoord() const;
		long	getEdgeOriCoord() const;
		long	getCornerPermCoord() const;
		long	getEdgePermCoord() const;
		long	getUDSliceCoord() const;
		// coordinates setters
		void	setCornerOriCoord(long coordinate);
		void	setEdgeOriCoord(long coordinate);
		void	setCornerPermCoord(long coordinate);
		void	setEdgePermCoord(long coordinate);
		void	setUDSliceCoord(long coordinate);
		
    private:

		CornerCubie	_corners[CORNER_COUNT];
		EdgeCubie	_edges[EDGE_COUNT];
};

#endif
