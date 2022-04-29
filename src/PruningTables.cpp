/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:21:48 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/30 01:03:27 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PruningTables.hpp"

PruningTables* PruningTables::_instance = NULL;

PruningTables::Generator::Generator(
	const std::string& name,
	const std::vector<Move>& allowedMoves,
	size_t max,
	const std::vector<std::vector<int>>& mt
) : BaseGenerator(name, max), allowedMoves(allowedMoves), moveTable(mt) {}

PruningTables::PruningTables(const MoveTables& mt) :
	ATable(std::vector<BaseGenerator*>({
		new Generator(
			std::string("corner_ori.prune"),
			Rubik::GroupP1,
			CORN_ORI_MAX,
			mt[Table::CORNER_ORI]
		),
		new Generator(
			std::string("edge_ori.prune"),
			Rubik::GroupP1,
			EDGE_ORI_MAX,
			mt[Table::EDGE_ORI]
		),
		new Generator(
			std::string("ud_slice.prune"),
			Rubik::GroupP1,
			UD_SLICE_MAX,
			mt[Table::UD_SLICE]
		),
		new Generator(
			std::string("corner_perm.prune"),
			Rubik::GroupP2,
			CORN_PERM_MAX,
			mt[Table::CORNER_PERM]
		),
		new Generator(
			std::string("edge_perm_p2.prune"),
			Rubik::GroupP2,
			EDGE_P2_PERM_MAX,
			mt[Table::EDGE_P2]
		),
		new Generator(
			std::string("ud_slice_p2.prune"),
			Rubik::GroupP2,
			UD_SLICE_P2_MAX,
			mt[Table::UD_SLICE_P2]
		)
	}))
{
	for (size_t i = 0; i < _generators.size(); i++) {
		_tables[i].resize(_generators[i]->max + 1, -1);
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
	int			buf;

	for (size_t i = 0; i <= gen->max; i++)
	{
		if (read(fd, &buf, sizeof(int)) != sizeof(int))
			throw std::exception();
		_tables[index][i] = buf;
	}
}

void
PruningTables::_generate(int index, int fd)
{
	Generator*			gen		= reinterpret_cast<Generator*>(_generators[index]);
	std::vector<int>&	table	= _tables[index];
	std::list<int>*		buffer	= new std::list<int>(1, 0);
	std::list<int>*		tmp		= new std::list<int>();
	size_t				filled	= 0;
	size_t				depth	= 0;

	while (filled < table.size() / 2)
	{
		for (
			std::list<int>::const_iterator it = buffer->begin();
			it != buffer->end();
			it++
		) {
			if (table[*it] == -1)
			{
				table[*it] = depth;
				filled++;
			}

			for (
				std::vector<Move>::const_iterator m = gen->allowedMoves.begin();
				m != gen->allowedMoves.end();
				m++
			) {
				int newCoord = gen->moveTable[*it][*m];
				if (table[newCoord] == -1)
					tmp->push_back(newCoord);
			}
		}

		std::swap(buffer, tmp);
		tmp->clear();
		depth++;
	}
	
	delete tmp;
	buffer->clear();
	for (size_t i = 1; i < table.size(); i++)
	{
		if (table[i] == -1)
			buffer->push_back(i);
	}

	while (buffer->size())
	{
		std::list<int>::const_iterator it = buffer->begin();
		
		while (it != buffer->end())
		{
			bool found = false;

			for (
				std::vector<Move>::const_iterator m = gen->allowedMoves.begin();
				m != gen->allowedMoves.end() && !found;
				m++
			) {
				int newCoord = gen->moveTable[*it][*m];
				if (table[newCoord] >= 0)
				{
					found = true;
					table[*it] = table[newCoord] + 1;
					it = buffer->erase(it);
					filled++;
				}
			}

			if (!found)
				it++;
		}
	}
	delete buffer;

	for (size_t i = 0; i < table.size(); i++) {
		if (write(fd, table.data() + i, sizeof(int)) != sizeof(int))
			throw std::exception();
	}
}
