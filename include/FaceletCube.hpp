/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:05:33 by user42            #+#    #+#             */
/*   Updated: 2022/04/24 19:29:07 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACELETCUBE_HPP
# define FACELETCUBE_HPP

# include "ACube.hpp"

class FaceletCube : public ACube
{
	public:

		// constructors
		FaceletCube(const std::list<std::string>& sequence = {});	// default
		FaceletCube(const FaceletCube& rhs);    					// copy

		// destructors
		~FaceletCube();

		// operators
		FaceletCube&	operator=(const FaceletCube& rhs);

		// member functions
		virtual void	move(char face, int factor = 1);
		void			render() const;

	private:

		std::vector<Facelet>	_facelets;
};

#endif
