/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Facelet.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:01:37 by user42            #+#    #+#             */
/*   Updated: 2022/04/21 15:32:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FACELET_HPP
# define FACELET_HPP

/*
**  |1|2|3|
**  |4|5|6| Distribution of the 9 facelets on a face
**  |7|8|9|
*/

enum Facelet {
	U1, U2, U3, U4, U5, U6, U7, U8, U9, // UP
	L1, L2, L3, L4, L5, L6, L7, L8, L9, // LEFT
	F1, F2, F3, F4, F5, F6, F7, F8, F9, // FRONT
	R1, R2, R3, R4, R5, R6, R7, R8, R9, // RIGHT
	B1, B2, B3, B4, B5, B6, B7, B8, B9, // BACK
	D1, D2, D3, D4, D5, D6, D7, D8, D9, // DOWN
};

#endif
