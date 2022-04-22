/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACube.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/22 23:23:27 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 23:26:06 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ACube.hpp"

void
ACube::scramble(const std::list<std::string>& sequence)
{
	for (
		std::list<std::string>::const_iterator it = sequence.begin();
		it != sequence.end();
		it++
	) {
		try {
			const char	singmaster	= it->at(0);
			int			factor		= 1;

			if (it->length() == 2)
			{
				if (it->at(1) == '2')
					factor = 2; // half-turn
				else if (it->at(1) == '\'')
					factor = 3; // counter clockwise
				else
					throw std::exception(); // not an existing move
			}
			else if (it->length() != 1)
				throw std::exception(); // not an existing move

			for (int i = 0; i < factor; i++)
				this->move(singmaster, factor); // execute the move

		} catch (const std::exception& e) {
			throw std::invalid_argument("Scramble is not valid.");
		}
	}
}
