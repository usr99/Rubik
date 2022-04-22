/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACube.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 23:21:04 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 23:33:00 by user42           ###   ########.fr       */
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
