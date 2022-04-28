/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rubik.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 17:05:11 by user42            #+#    #+#             */
/*   Updated: 2022/04/28 16:20:41 by mamartin         ###   ########.fr       */
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

# define FACES_COUNT		6
# define MOVES_COUNT		18
# define FACELET_COUNT		54
# define CORNER_COUNT		8
# define EDGE_COUNT			12

# define CORN_ORI_MAX		2186		// 3^7 - 1
# define EDGE_ORI_MAX		2047		// 2^11 - 1
# define CORN_PERM_MAX		40319		// 8! - 1
# define EDGE_PERM_MAX		479001599	// 12! - 1
# define UD_SLICE_MAX		494			// 12*11*10*9/4! - 1
# define EDGE_P2_PERM_MAX	40319		// 8! - 1
# define UD_SLICE_P2_MAX	23			// 4! - 1

# define TABLES_COUNT		6

enum Move {
	Ux1, Ux2, Ux3,
	Rx1, Rx2, Rx3,
	Fx1, Fx2, Fx3,
	Dx1, Dx2, Dx3,
	Lx1, Lx2, Lx3,
	Bx1, Bx2, Bx3, NONE
};

enum Facelet {
	U1, U2, U3, U4, U5, U6, U7, U8, U9, // UP
	R1, R2, R3, R4, R5, R6, R7, R8, R9, // RIGHT
	F1, F2, F3, F4, F5, F6, F7, F8, F9, // FRONT
	D1, D2, D3, D4, D5, D6, D7, D8, D9, // DOWN
	L1, L2, L3, L4, L5, L6, L7, L8, L9, // LEFT
	B1, B2, B3, B4, B5, B6, B7, B8, B9  // BACK
};

enum Corner {
	URF, UFL,
	ULB, UBR,
	DFR, DLF,
	DBL, DRB
};

enum Edge {
	UR, UF, UL,
	UB, DR, DF,
	DL, DB, FR,
	FL, BL, BR
};

enum Table {
	CORNER_ORI, EDGE_ORI, UD_SLICE,
	CORNER_PERM, EDGE_P2, UD_SLICE_P2
};

namespace Rubik {

	// Map a corner to their facelets
	static const Facelet CornerFacelets[CORNER_COUNT][3] = {
		{ U9, R1, F3 }, { U7, F1, L3 },
		{ U1, L1, B3 }, { U3, B1, R3 },
		{ D3, F9, R7 }, { D1, L9, F7 },
		{ D7, B9, L7 }, { D9, R9, B7 }
	};

	// Map an edge to their facelets
	static const Facelet EdgeFacelets[EDGE_COUNT][3] = {
		{ U6, R2 }, { U8, F2 }, { U4, L2 },
		{ U2, B2 }, { D6, R8 }, { D2, F8 },
		{ D4, L8 }, { D8, B8 }, { F6, R4 },
		{ F4, L6 }, { B6, L4 }, { B4, R6 }
	};

	// Map a char to a face
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

// math_utils.cpp
unsigned int	factorial(unsigned int n);
unsigned int	binomial(unsigned int n, unsigned int k);

#endif
