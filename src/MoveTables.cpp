/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MoveTables.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 22:30:54 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/29 17:25:10 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/MoveTables.hpp"

MoveTables::Generator::Generator(
	const std::string& name,
	void (CubieCube::*set)(int),
	int (CubieCube::*get)(void) const,
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
		),
		Generator(
			std::string("corner_perm.move"),
			&CubieCube::setCornerPermCoord,
			&CubieCube::getCornerPermCoord,
			CORN_PERM_MAX
		),		
		Generator(
			std::string("edge_perm_p2.move"),
			&CubieCube::setPhase2EdgePermCoord,
			&CubieCube::getPhase2EdgePermCoord,
			EDGE_P2_PERM_MAX
		),
		Generator(
			std::string("ud_slice_p2.move"),
			&CubieCube::setPhase2UDSliceCoord,
			&CubieCube::getPhase2UDSliceCoord,
			UD_SLICE_P2_MAX
		)
	})
{
	struct stat	st;

	if (stat("./tables", &st) == -1)
	{
		if (errno == ENOENT) // folder not found
		{
			std::cout << "Creating tables folder...\n";
			if (mkdir("./tables", S_IRWXU | S_IRWXG | S_IROTH) == -1)
				throw std::runtime_error("Could not create ./tables folder");
		}
		else
			throw std::runtime_error("Error when trying to access move tables");
	}

	for (int i = 0; i < TABLES_COUNT; i++)
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
		if (errno == ENOENT) // table not found
		{
			std::cout << "Generating " + path + " ...\n";
			_generate(index);
			return ;
		}
		else {
			throw std::runtime_error("Could not open " + _generators[index].name);
		}
	}

	int	buf;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1)
		throw std::runtime_error(std::string("Could not load ") + path);

	std::cout << "Loading " + path + " ...\n";
	for (int i = 0; i <= _generators[index].max; i++)
	{
		tables[index][i].reserve(MOVES_COUNT);
		for (int j = 0; j < MOVES_COUNT ; j++)
		{
			if (read(fd, &buf, sizeof(int)) == -1)
				throw std::runtime_error(std::string("Could not load ") + path);
			tables[index][i][j] = buf;
		}
	}
	close(fd);
}

void
MoveTables::_generate(int index)
{
	const Generator&	gen(_generators[index]);
	std::string			filename("./tables/" + gen.name);
	CubieCube			cube;
	
	int	fd = open(filename.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);
	if (fd == -1)
		throw std::runtime_error(std::string("Could not generate ") + filename);

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
				const int newCoord = (cube.*gen.get)(); // store the new coordinate

				if (k != 3) // k == 3 restores the initial state
				{
					// write it in the table
					if (write(fd, &newCoord, sizeof(int)) == -1)
						throw std::runtime_error(std::string("Could not generate ") + filename);
					tables[index][i][f * 3 + k] = newCoord;
				}
			}
		}
	}
	close(fd);
}
