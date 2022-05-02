/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:21:48 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 16:16:23 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PruningTables.hpp"

PruningTables* PruningTables::_instance = NULL;

PruningTables::Generator::Generator(
	const std::string& name,
	size_t size,
	const std::vector<Move>& allowedMoves,
	unsigned int mt1,
	unsigned int mt2
) : BaseGenerator(name, size), allowedMoves(allowedMoves), mtIndex1(mt1), mtIndex2(mt2) {}

PruningTables::PruningTables(const MoveTables& mt) :
	ATable(std::vector<BaseGenerator*>({
		new Generator(
			std::string("orientation.prune"),
			PRUN_SZ_ORI,
			Rubik::GroupP1,
			MoveTables::CORNER_ORI,
			MoveTables::EDGE_ORI
		),
		new Generator(
			std::string("edge_p1.prune"),
			PRUN_SZ_EDGE_P1,
			Rubik::GroupP1,
			MoveTables::EDGE_ORI,
			MoveTables::UD_SLICE
		),
		new Generator(
			std::string("corner_udslice_perm_p2.prune"),
			PRUN_SZ_CORN_UD_P2,
			Rubik::GroupP2,
			MoveTables::CORNER_PERM,
			MoveTables::UD_SLICE_P2
		),
		new Generator(
			std::string("edge_udslice_perm_p2.prune"),
			PRUN_SZ_EDGE_UD_P2,
			Rubik::GroupP2,
			MoveTables::EDGE_P2,
			MoveTables::UD_SLICE_P2
		)
	})), _mt(mt)
{
	for (size_t i = 0; i < _generators.size(); i++) {
		_create(i);
	}
}

PruningTables*	PruningTables::getInstance(const MoveTables& mt)
{
	if (!_instance)
		_instance = new PruningTables(mt);
	return _instance;
}

void
PruningTables::destroy()
{
	if (_instance)
		delete _instance;
}

PruningTables::~PruningTables() {}

void
PruningTables::_load(int index, int fd)
{
	Generator*	gen			= reinterpret_cast<Generator*>(_generators[index]);
	size_t		mtSizes[2]	= { _mt[gen->mtIndex1].capacity(), _mt[gen->mtIndex2].capacity() };
	int8_t		buf;

	// load table data from file
	_tables[index].resize(mtSizes[0]);
	for (u_int i = 0; i < mtSizes[0]; i++)
	{
		_tables[index][i].reserve(mtSizes[1]);
		for (u_int j = 0; j < mtSizes[1]; j++)
		{
			if (read(fd, &buf, sizeof(int8_t)) != sizeof(int8_t))
				throw std::exception();
			_tables[index][i][j] = buf;
		}
	}
}

void
PruningTables::_generate(int index, int fd)
{
	Generator*							gen			= reinterpret_cast<Generator*>(_generators[index]);
	size_t								mtSizes[2]	= { _mt[gen->mtIndex1].capacity(), _mt[gen->mtIndex2].capacity() };
	std::vector<std::vector<int8_t>>&	table		= _tables[index];

	int8_t								depth		= 0;
	u_int								filled		= 1;
	bool								backsearch	= false;

	// allocate table
	_tables[index].resize(mtSizes[0]);
	for (u_int i = 0; i < mtSizes[0]; i++)
		_tables[index][i].resize(mtSizes[1], -1);

	table[0][0] = 0; // solved state is at depth 0
	while (filled != gen->size)
	{
		if (depth == 9) // backward search is faster for depth >= 9
			backsearch = true;

		u_int i = 0;
		while (i < mtSizes[0])
		{
			u_int j = 0;
			while (j < mtSizes[1])
			{
				bool match;

				if (backsearch)
					match = (table[i][j] == -1); // empty entry
				else
					match = (table[i][j] == depth);

				if (match)
				{
					for ( // each move
						std::vector<Move>::const_iterator m = gen->allowedMoves.begin();
						m != gen->allowedMoves.end();
						m++
					) {
						coordinates_t coords = std::make_pair(
							_mt[gen->mtIndex1][i][*m],
							_mt[gen->mtIndex2][j][*m]
						); // generate the new coordinates resulting from the move

						if (!backsearch)
						{
							if (table[coords.first][coords.second] == -1) // empty entry
							{
								table[coords.first][coords.second] = depth + 1;
								filled++;
							}
						}
						else
						{
							if (table[coords.first][coords.second] == depth)
							{
								table[i][j] = depth + 1;
								filled++;
								break ;
							}
						}
					}
				}
				j++;
			}
			i++;
		}
		depth++;
	}

	// write table data in a file
	for (u_int i = 0; i < mtSizes[0]; i++)
	{
		for (u_int j = 0; j < mtSizes[1]; j++)
		{
			if (write(fd, table[i].data() + j, sizeof(int8_t)) != sizeof(int8_t))
				throw std::exception();
		}
	}
}
