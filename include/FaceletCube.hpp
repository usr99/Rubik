/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:05:33 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 01:33:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACELETCUBE_HPP
# define FACELETCUBE_HPP

# include "rubik.hpp"
# include "Facelet.hpp"
# include "Move.hpp"

class FaceletCube
{
	public:

		// constructors
		FaceletCube(const std::list<std::string>& scramble = {});	// default
		FaceletCube(const FaceletCube& rhs);    					// copy

		// destructors
		~FaceletCube();

		// operators
		FaceletCube& operator=(const FaceletCube& rhs);

		// functions
		void	move(char face, int factor = 1);
		void	render();

	private:

		Facelet					_corners[CORNER_COUNT][3];
		Facelet					_edges[EDGE_COUNT][2];
		std::vector<Facelet>	_data;
};

#endif
