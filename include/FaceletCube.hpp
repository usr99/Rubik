/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:05:33 by user42            #+#    #+#             */
/*   Updated: 2022/04/21 15:32:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACELETCUBE_HPP
# define FACELETCUBE_HPP

# include "Facelet.hpp"

class FaceletCube
{
	public:

		// constructors
		FaceletCube();                          // default
		FaceletCube(const FaceletCube& rhs);    // copy

		// destructors
		~FaceletCube();

		// operators
		FaceletCube& operator=(const FaceletCube& rhs);

	private:

		Facelet _corners[8][3];
		Facelet _edges[12][2];
};

#endif
