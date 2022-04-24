/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubie.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:15:19 by user42            #+#    #+#             */
/*   Updated: 2022/04/24 19:36:01 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBIE_HPP
# define CUBIE_HPP

# include "rubik.hpp" // contains Corner and Edge enums

using CornerCubie = struct CornerCubie {

	CornerCubie();
	CornerCubie(const Corner c, const char o);

	Corner	c;
	char	o; // orientation ([0;2])
};

using EdgeCubie = struct EdgeCubie {

	EdgeCubie();
	EdgeCubie(const Edge e, const char o);

	Edge	e;
	char	o; // orientation ([0;1])
};

#endif
