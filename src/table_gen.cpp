/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_gen.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 02:09:40 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 15:16:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CubieCube.hpp"

void generateMoveTables()
{
	std::ofstream	ofs("./tables/corner_ori.move.table");
	char			faces[6] = { 'U', 'R', 'F', 'D', 'L', 'B' };
	CubieCube		cube;

	for (int i = 0; i < 2187; i++)
	{
		cube.setCornerOriCoord(i);

		for (int f = 0; f < 6; f++)
		{
			for (int k = 0; k <= 3; k++)
			{
				cube.move(faces[f]);
				const int coord = cube.getCornerOriCoord();

				if (k != 3)
					ofs.write(reinterpret_cast<const char*>(&coord), sizeof(int));
			}
		}
	}
}

int**	loadMoveTables()
{
	
}
