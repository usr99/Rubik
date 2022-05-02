/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MoveTables.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 22:30:54 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 16:15:56 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/MoveTables.hpp"

MoveTables* MoveTables::_instance = NULL;

MoveTables::Generator::Generator(
	const std::string &name,
	size_t size,
	void (CubieCube::*set)(u_int16_t),
	u_int16_t (CubieCube::*get)(void) const
) : BaseGenerator(name, size), set(set), get(get) {}

MoveTables::MoveTables() :
	ATable(std::vector<BaseGenerator*>({
		new Generator(
			std::string("corner_ori.move"),
			CORN_ORI_MAX+1,
			&CubieCube::setCornerOriCoord,
			&CubieCube::getCornerOriCoord
		),
		new Generator(
			std::string("edge_ori.move"),
			EDGE_ORI_MAX+1,
			&CubieCube::setEdgeOriCoord,
			&CubieCube::getEdgeOriCoord
		),
		new Generator(
			std::string("ud_slice.move"),
			UD_SLICE_MAX+1,
			&CubieCube::setUDSliceCoord,
			&CubieCube::getUDSliceCoord
		),
		new Generator(
			std::string("corner_perm.move"),
			CORN_PERM_MAX+1,
			&CubieCube::setCornerPermCoord,
			&CubieCube::getCornerPermCoord
		),		
		new Generator(
			std::string("edge_perm_p2.move"),
			EDGE_P2_PERM_MAX+1,
			&CubieCube::setPhase2EdgePermCoord,
			&CubieCube::getPhase2EdgePermCoord
		),
		new Generator(
			std::string("ud_slice_p2.move"),
			UD_SLICE_P2_MAX+1,
			&CubieCube::setPhase2UDSliceCoord,
			&CubieCube::getPhase2UDSliceCoord
		)
	}))
{
	for (size_t i = 0; i < _generators.size(); i++) {
		_tables[i].resize(_generators[i]->size);
		_create(i);
	}
}

MoveTables*	MoveTables::getInstance()
{
	if (!_instance)
		_instance = new MoveTables();
	return _instance;
}

void
MoveTables::destroy()
{
	if (_instance)
		delete _instance;
}

MoveTables::~MoveTables() {}

void
MoveTables::_load(int index, int fd)
{
	Generator*	gen = reinterpret_cast<Generator*>(_generators[index]);
	u_int16_t	buf;

	for (size_t i = 0; i < gen->size; i++)
	{
		_tables[index][i].reserve(MOVES_COUNT);
		for (int j = 0; j < MOVES_COUNT ; j++)
		{
			if (read(fd, &buf, sizeof(u_int16_t)) != sizeof(u_int16_t))
				throw std::exception();
			_tables[index][i][j] = buf;
		}
	}
}

void
MoveTables::_generate(int index, int fd)
{
	Generator*	gen = reinterpret_cast<Generator*>(_generators[index]);
	CubieCube	cube;

	for (size_t i = 0; i < gen->size; i++)
	{
		_tables[index][i].reserve(MOVES_COUNT);

		// generate a permutation with coordinate i
		(cube.*gen->set)(i);

		for (int f = 0; f < FACES_COUNT; f++)
		{
			for (int k = 0; k <= 3; k++)
			{
				cube.move(Rubik::Faces[f]); // apply all 18 face turns
				const int newCoord = (cube.*gen->get)(); // store the new coordinate

				if (k != 3) // k == 3 restores the initial state
				{
					// write it in the table
					if (write(fd, &newCoord, sizeof(u_int16_t)) != sizeof(u_int16_t))
						throw std::exception();
					_tables[index][i][f * 3 + k] = newCoord;
				}
			}
		}
	}	
}
