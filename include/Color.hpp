/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 17:35:22 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/11 20:45:11 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENUMS_HPP
# define ENUMS_HPP

# include <list>

class Color {

	public:

		using iterator = std::list<int>::const_iterator;

		static iterator		begin();
		static iterator		end();

		static const int	WHITE;
		static const int	ORANGE;
		static const int	GREEN;
		static const int	RED;
		static const int	BLUE;
		static const int	YELLOW;

	private:

		Color();

		static const std::list<int>	_All;
};

#endif
