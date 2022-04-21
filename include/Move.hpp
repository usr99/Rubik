/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Move.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 16:25:41 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 01:32:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVE_HPP
# define MOVE_HPP

# include <map>
# include <vector>

# include "Facelet.hpp"
# include "Cubie.hpp"

namespace Move {

	static const std::map<char, std::vector<Facelet>> faceletTable = { // "is carried to" representation
		std::make_pair('U', std::vector<Facelet>({ // U
			U3, U6, U9, U2, U5, U8, U1, U4, U7,
			F1, F2, F3, R4, R5, R6, R7, R8, R9,
			L1, L2, L3, F4, F5, F6, F7, F8, F9,
			D1, D2, D3, D4, D5, D6, D7, D8, D9,
			B1, B2, B3, L4, L5, L6, L7, L8, L9,
			R1, R2, R3, B4, B5, B6, B7, B8, B9
		})),
		std::make_pair('R', std::vector<Facelet>({ // R
			U1, U2, B7, U4, U5, B4, U7, U8, B1,
			R3, R6, R9, R2, R5, R8, R1, R4, R7,
			F1, F2, U3, F4, F5, U6, F7, F8, U9,
			D1, D2, F3, D4, D5, F6, D7, D8, F9,
			L1, L2, L3, L4, L5, L6, L7, L8, L9,
			D9, B2, B3, D6, B5, B6, D3, B8, B9
		})),
		std::make_pair('F', std::vector<Facelet>({ // F
			U1, U2, U3, U4, U5, U6, R1, R4, R7,
			D3, R2, R3, D2, R5, R6, D1, R8, R9,
			F3, F6, F9, F2, F5, F8, F1, F4, F7,
			L3, L6, L9, D4, D5, D6, D7, D8, D9,
			L1, L2, U9, L4, L5, U8, L7, L8, U7,
			B1, B2, B3, B4, B5, B6, B7, B8, B9
		})), 
		std::make_pair('D',  std::vector<Facelet>({ // D
			U1, U2, U3, U4, U5, U6, U7, U8, U9,
			R1, R2, R3, R4, R5, R6, B7, B8, B9,
			F1, F2, F3, F4, F5, F6, R7, R8, R9,
			D3, D6, D9, D2, D5, D8, D1, D4, D7,
			L1, L2, L3, L4, L5, L6, F7, F8, F9,
			B1, B2, B3, B4, B5, B6, L7, L8, L9
		})), 
		std::make_pair('L',  std::vector<Facelet>({ // L
			F1, U2, U3, F4, U5, U6, F7, U8, U9,
			R1, R2, R3, R4, R5, R6, R7, R8, R9,
			D1, F2, F3, D4, F5, F6, D7, F8, F9,
			B9, D2, D3, B6, D5, D6, B3, D8, D9,
			L3, L6, L9, L2, L5, L8, L1, L4, L7,
			B1, B2, U7, B4, B5, U4, B7, B8, U1
		})), 
		std::make_pair('B',  std::vector<Facelet>({ // B
			L7, L4, L1, U4, U5, U6, U7, U8, U9,
			R1, R2, U1, R4, R5, U2, R7, R8, U3,
			F1, F2, F3, F4, F5, F6, F7, F8, F9,
			D1, D2, D3, D4, D5, D6, R9, R6, R3,
			D7, L2, L3, D8, L5, L6, D9, L8, L9,
			B3, B6, B9, B2, B5, B8, B1, B4, B7
		}))
	};

	namespace Cubie {
		namespace Corner {

		}

		namespace Edge {

		}
	}
}

#endif
