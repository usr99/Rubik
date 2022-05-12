/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:05:33 by user42            #+#    #+#             */
/*   Updated: 2022/05/11 21:51:29 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACELETCUBE_HPP
# define FACELETCUBE_HPP

# include "ACube.hpp"

class FaceletCube : public ACube
{
	public:

		// constructors
		FaceletCube(const std::list<std::string>& sequence = {});			// default
		FaceletCube(const std::array<Facelet, FACELET_COUNT>& facelets);	// copy
		FaceletCube(const FaceletCube& rhs);    							// copy

		// destructors
		~FaceletCube();

		// operators
		FaceletCube&	operator=(const FaceletCube& rhs);

		// member functions
		virtual void	move(char face, int factor = 1);
		void			render() const;

		const std::array<Facelet, FACELET_COUNT>& getFacelets() const;

	private:

		std::array<Facelet, FACELET_COUNT>	_facelets;
};

#endif
