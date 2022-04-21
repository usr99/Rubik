/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rubik.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 17:05:11 by user42            #+#    #+#             */
/*   Updated: 2022/04/22 01:32:23 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEF_HPP
# define DEF_HPP

# define FACELET_COUNT	54
# define CORNER_COUNT	8
# define EDGE_COUNT		12

namespace Rubik {

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

#endif
