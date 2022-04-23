/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubieCube.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:29:37 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 01:58:04 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBIECUBE_HPP
# define CUBIECUBE_HPP

# include "ACube.hpp"
# include "Cubie.hpp"

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
		unsigned int	getCornerOriCoord() const;
		unsigned int	getEdgeOriCoord() const;
		unsigned int	getCornerPermCoord() const;
		unsigned int	getEdgePermCoord() const;
		// coordinates setters
		void			setCornerOriCoord(int coordinate);
		void			setEdgeOriCoord(int coordinate);
		void			setCornerPermCoord(int coordinate);
		void			setEdgePermCoord(int coordinate);
		
    private:

		CornerCubie	_corners[CORNER_COUNT];
		EdgeCubie	_edges[EDGE_COUNT];
};

#endif
