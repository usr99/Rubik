/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACube.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 17:53:13 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/23 17:53:14 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACUBE_HPP
# define ACUBE_HPP

# include "rubik.hpp"

class ACube
{
	public:

		void scramble(const std::list<std::string>& sequence);
		virtual void move(char face, int factor = 1) = 0;
};

#endif
