/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 17:05:11 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 01:16:57 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEF_HPP
# define DEF_HPP

# include <map>

# define FACELET_COUNT	54
# define CORNER_COUNT	8
# define EDGE_COUNT		12

enum TurnAxis { U, R, F, D, L, B };

namespace Rubik {

	// Map a color to a face (same order as TurnAxis above)
	static const u_char ColorScheme[6] = { 231, 160, 28, 11, 202, 20 };
	
	// Map a move to the character representing it
	static const std::map<char, TurnAxis> SingmasterNotation = {
		{ 'U', U },
		{ 'R', R },
		{ 'F', F },
		{ 'D', D },
		{ 'L', L },
		{ 'B', B },
	};
}

#endif
