/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MoveTables.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 22:30:54 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/25 22:48:55 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/stat.h>
#include <cstring>
#include "../include/MoveTables.hpp"

MoveTables::Generator::Generator(
	const std::string& name,
	void (CubieCube::*set)(long),
	long (CubieCube::*get)(void) const,
	int max	
) : name(name), set(set), get(get), max(max) {}

MoveTables::MoveTables() :
	_generators({
		Generator(
			std::string("corner_ori.move"),
			&CubieCube::setCornerOriCoord,
			&CubieCube::getCornerOriCoord,
			CORN_ORI_MAX
		),
		Generator(
			std::string("edge_ori.move"),
			&CubieCube::setEdgeOriCoord,
			&CubieCube::getEdgeOriCoord,
			EDGE_ORI_MAX
		),
		Generator(
			std::string("ud_slice.move"),
			&CubieCube::setUDSliceCoord,
			&CubieCube::getUDSliceCoord,
			UD_SLICE_MAX
		)
	})
{
	struct stat	st;

	if (stat("./tables", &st) == -1)
	{
		if (errno == ENOENT)
		{
			std::cout << "Creating tables folder...\n";
			if (mkdir("./tables", S_IRWXU | S_IRWXG | S_IROTH) == -1)
				throw std::runtime_error("Could not create ./tables folder");
		}
		else
			throw std::runtime_error("Error when trying to access move tables");
	}

	for (int i = 0; i < MOVE_TABLES_COUNT; i++)
	{
		tables[i].reserve(_generators[i].max + 1);
		_load(i);
	}
}

void
MoveTables::_load(int index)
{
	const std::string	path("./tables/" + _generators[index].name);
	struct stat			st;

	if (stat(path.c_str(), &st) == -1)
	{
		if (errno == ENOENT)
		{
			std::cout << "Generating " + path + " ...\n";
			_generate(index);
			return ;
		}
		else {
			std::cerr << strerror(errno) << "\n";
			throw std::runtime_error("Could not open " + _generators[index].name);
		}
	}

	std::ifstream	ifs(path);
	char*			buf = new char[sizeof(long)];

	std::cout << "Loading " + path + " ...\n";
	for (int i = 0; i <= _generators[index].max; i++)
	{
		tables[index][i].reserve(MOVES_COUNT);
		for (int j = 0; j < MOVES_COUNT ; j++)
		{
			ifs.read(buf, sizeof(long));
			tables[index][i][j] = *(reinterpret_cast<long*>(buf));
		}
	}

	delete buf;
	ifs.close();
}

void
MoveTables::_generate(int index)
{
	const Generator&	gen(_generators[index]);
	std::string			filename("./tables/" + gen.name);
	std::ofstream		ofs(filename);
	CubieCube			cube;
	
	for (int i = 0; i <= gen.max; i++)
	{
		tables[index][i].reserve(MOVES_COUNT);

		// generate a permutation with coordinate i
		(cube.*gen.set)(i);

		for (int f = 0; f < FACES_COUNT; f++)
		{
			for (int k = 0; k <= 3; k++)
			{
				cube.move(Rubik::Faces[f]); // apply all 18 face turns
				const long newCoord = (cube.*gen.get)(); // store the new coordinate

				if (k != 3) // write it in the table
				{
					ofs.write(reinterpret_cast<const char*>(&newCoord), sizeof(long));
					tables[index][i][f * 3 + k] = newCoord;
				}
			}
		}
	}
	ofs.close();
}
