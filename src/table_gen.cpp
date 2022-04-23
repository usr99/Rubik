/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table_gen.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 02:09:40 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 22:02:36 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CubieCube.hpp"

void generateMoveTable(
	const std::string& tableName,
	void (CubieCube::*set)(int),
	unsigned int (CubieCube::*get)(void) const,
	int max
) {

	std::string		filename("./tables/" + tableName + ".move.table");
	std::ofstream	ofs(filename);
	CubieCube		cube;

	std::cout << "Generating " + filename + " ...\n";

	for (int i = 0; i <= max; i++)
	{
		// generate a permutation with coordinate i
		(cube.*set)(i);

		for (int f = 0; f < 6; f++)
		{
			for (int k = 0; k <= 3; k++)
			{
				cube.move(Rubik::Faces[f]); // apply all 18 face turns
				const int newCoord = (cube.*get)(); // store the new coordinate

				if (k != 3) // write it in the table
					ofs << newCoord << "\n";
					//ofs.write(reinterpret_cast<const char*>(&newCoord), sizeof(int));
			}
		}
	}
}

void generateMoveTables()
{
	generateMoveTable(
		std::string("corner_ori"),
		&CubieCube::setCornerOriCoord,
		&CubieCube::getCornerOriCoord,
		CORN_ORI_MAX
	);

	generateMoveTable(
		std::string("edge_ori"),
		&CubieCube::setEdgeOriCoord,
		&CubieCube::getEdgeOriCoord,
		EDGE_ORI_MAX
	);

	generateMoveTable(
		std::string("ud_slice_phase1"),
		&CubieCube::setUDSliceCoord,
		&CubieCube::getUDSliceCoord,
		UD_SLICE_MAX
	);
}

int** loadMoveTables()
{

}
