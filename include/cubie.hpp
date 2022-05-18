/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubie.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:15:19 by user42            #+#    #+#             */
/*   Updated: 2022/05/19 00:55:33 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBIE_HPP
# define CUBIE_HPP

# include "rubik.hpp" // contains Corner and Edge enums

using CornerCubie = struct CornerCubie {

	CornerCubie();
	CornerCubie(const Corner c, const char o);

	Corner get() const;

	Corner	c;
	char	o; // orientation ([0;2])
};

using EdgeCubie = struct EdgeCubie {

	EdgeCubie();
	EdgeCubie(const Edge e, const char o);

	Edge get() const;

	Edge	e;
	char	o; // orientation ([0;1])
};

#endif
