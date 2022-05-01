/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:21:48 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 00:45:34 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PruningTables.hpp"

PruningTables* PruningTables::_instance = NULL;

PruningTables::Generator::Generator(
	const std::string& name,
	const std::vector<Move>& allowedMoves,
	const std::vector<std::vector<int>>& mt1,
	const std::vector<std::vector<int>>& mt2
) : BaseGenerator(name, 0),
	allowedMoves(allowedMoves), moveTable1(mt1), moveTable2(mt2) {}

PruningTables::PruningTables(const MoveTables& mt) :
	ATable(std::vector<BaseGenerator*>({
		new Generator(
			std::string("orientation.prune"),
			Rubik::GroupP1,
			mt[Table::CORNER_ORI],
			mt[Table::EDGE_ORI]
		),
		new Generator(
			std::string("ud_slice.prune"),
			Rubik::GroupP1,
			mt[Table::EDGE_ORI],
			mt[Table::UD_SLICE]
		),
		new Generator(
			std::string("corner_udslice_perm_p2.prune"),
			Rubik::GroupP2,
			mt[Table::CORNER_PERM],
			mt[Table::UD_SLICE_P2]
		),
		new Generator(
			std::string("edge_udslice_perm_p2.prune"),
			Rubik::GroupP2,
			mt[Table::EDGE_P2],
			mt[Table::UD_SLICE_P2]
		)
	}))
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

PruningTables::~PruningTables() {}

void
PruningTables::_load(int index, int fd)
{
	Generator*	gen = reinterpret_cast<Generator*>(_generators[index]);
	pruning_t	buf;

	_tables[index].reserve(gen->moveTable1.capacity());
	for (u_int i = 0; i < gen->moveTable1.capacity(); i++)
	{
		_tables[index][i].reserve(gen->moveTable2.capacity());
		for (u_int j = 0; j < gen->moveTable2.capacity(); j++)
		{
			if (read(fd, &buf, sizeof(pruning_t)) != sizeof(pruning_t))
				throw std::exception();
			_tables[index][i][j] = buf;
		}
	}
}

void
PruningTables::_generate(int index, int fd)
{
	Generator*								gen			= reinterpret_cast<Generator*>(_generators[index]);
	std::vector<std::vector<pruning_t>>&	table		= _tables[index];

	pruning_t								depth		= 0;
	u_int									filled		= 1;
	bool									backsearch	= false;

	// allocate table
	_tables[index].reserve(gen->moveTable1.capacity());
	for (u_int i = 0; i < gen->moveTable1.capacity(); i++)
		_tables[index][i].resize(gen->moveTable2.capacity(), -1);
	u_int max = _tables[index].capacity() * _tables[index][0].capacity();

	table[0][0] = 0; // solved state is at depth 0
	while (filled != max)
	{
		if (depth == 9) // backward search is faster for depth >= 9
			backsearch = true;

		u_int i = 0;
		while (i < _tables[index].capacity())
		{
			u_int j = 0;
			while (j < _tables[index][0].capacity())
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
							gen->moveTable1[i][*m],
							gen->moveTable2[j][*m]
						);

						if (!backsearch)
						{
							if (table[coords.first][coords.second] == -1) // empty
							{
								table[coords.first][coords.second] = depth + 1;
								filled++;
								std::cout << filled << "\n";
							}
						}
						else
						{
							if (table[coords.first][coords.second] == depth)
							{
								table[i][j] = depth + 1;
								filled++;
								std::cout << filled << "\n";
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

	for (u_int i = 0; i < table.capacity(); i++)
	{
		for (u_int j = 0; j < table[i].capacity(); j++)
		{
			if (write(fd, table[i].data() + j, sizeof(pruning_t)) != sizeof(pruning_t))
				throw std::exception();
		}
	}
}
