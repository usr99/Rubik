/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FaceletCube.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 15:09:54 by user42            #+#    #+#             */
/*   Updated: 2022/04/21 15:32:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/FaceletCube.hpp"

FaceletCube::FaceletCube() :
	_corners({
		{ U9, R1, F3 },
		{ U7, F1, L3 },
		{ U1, L1, B3 },
		{ U3, B1, R3 },
		{ F9, R7, D3 },
		{ F7, D1, L9 },
		{ B9, L7, D7 },
		{ B7, R9, D9 }
	}),
	_edges({
		{ U6, R2 },
		{ U8, F2 },
		{ U4, L2 },
		{ U2, B2 },
		{ D6, R8 },
		{ D2, F8 },
		{ D4, L8 },
		{ D8, R8 },
		{ F6, R4 },
		{ F4, L6 },
		{ B4, R6 },
		{ B6, L4 }
	}) {}

FaceletCube::FaceletCube(const FaceletCube& rhs)
{
	*this = rhs;
}

FaceletCube::~FaceletCube() {}

FaceletCube&
FaceletCube::operator=(const FaceletCube& rhs)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
			_corners[i][j] = rhs._corners[i][j];
	}

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 2; j++)
			_edges[i][j] = rhs._edges[i][j];
	}
}
