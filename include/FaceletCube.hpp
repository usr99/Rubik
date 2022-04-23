/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:05:33 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 00:56:55 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACELETCUBE_HPP
# define FACELETCUBE_HPP

# include "ACube.hpp"
# include "Facelet.hpp"

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
