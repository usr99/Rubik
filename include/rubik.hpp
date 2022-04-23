/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rubik.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 17:05:11 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 22:03:06 by mamartin         ###   ########.fr       */
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

# define CORN_ORI_MAX	2186		// 3^7 - 1
# define EDGE_ORI_MAX	2047		// 2^11 - 1
# define CORN_PERM_MAX	40319		// 8! - 1
# define EDGE_PERM_MAX	479001599	// 12! - 1
# define UD_SLICE_MAX	494			// 12*11*10*9/4! - 1

namespace Rubik {

	enum Move {
		U, U2, U3,
		R, R2, R3,
		F, F2, F3,
		D, D2, D3,
		L, L2, L3,
		B, B2, B3
	};

	static const char Faces[6] = {
		'U', 'R', 'F', 'D', 'L', 'B'
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

// table_gen.cpp
int**			loadMoveTables();
void generateMoveTables();

// math_utils.cpp
unsigned long	factorial(unsigned int n);
unsigned long	binomial(unsigned int n, unsigned int k);

#endif
