/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rubik.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 17:05:11 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 14:54:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUBIK_HPP
# define RUBIK_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <list>
# include <vector>
# include <map>

# define FACELET_COUNT	54
# define CORNER_COUNT	8
# define EDGE_COUNT		12

namespace Rubik {

	enum Move {
		U, U2, U3,
		R, R2, R3,
		F, F2, F3,
		D, D2, D3,
		L, L2, L3,
		B, B2, B3
	};

	// Map a color to a face
	static const unsigned char ColorScheme[6] = {
		231,	// U
		160,	// R
		28,		// F
		11,		// D
		202,	// L
		20		// B
	};
}

void	generateMoveTables();
int**	loadMoveTables();

#endif
