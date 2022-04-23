/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cubie.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:15:19 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 23:28:38 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBIE_HPP
# define CUBIE_HPP

enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };
enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

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
	char	o; // is oriented ?
};

#endif