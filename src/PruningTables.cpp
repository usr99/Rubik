/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:21:48 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/28 16:07:48 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PruningTables.hpp"

PruningTables::Generator::Generator(
	const std::string&						name,
	const std::vector<std::vector<int>>&	mt
) : name(name), moveTable(mt) {}

PruningTables::PruningTables(const MoveTables& mt) : 
	_generators({
		Generator(
			std::string("corner_ori.prune"),
			mt.tables[Table::CORNER_ORI]
		),
		Generator(
			std::string("edge_ori.prune"),
			mt.tables[Table::EDGE_ORI]
		),
		Generator(
			std::string("ud_slice.prune"),
			mt.tables[Table::UD_SLICE]
		),
		Generator(
			std::string("corner_perm.prune"),
			mt.tables[Table::EDGE_P2]
		),		
		Generator(
			std::string("edge_perm_p2.prune"),
			mt.tables[Table::EDGE_P2]
		),
		Generator(
			std::string("ud_slice_p2.prune"),
			mt.tables[Table::UD_SLICE_P2]
		)								
	})
{
	for (int i = 0; i < TABLES_COUNT; i++)
	{
		tables[i].insert(tables[i].begin(), _generators[i].moveTable.capacity(), -1);
		_load(i);
	}
}

void
PruningTables::_load(int index)
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
			std::cerr << strerror(errno) << "\n";
			throw std::runtime_error("Could not open " + _generators[index].name);
		}
	}

	int	buf;
	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1)
		throw std::runtime_error(std::string("Could not load ") + path);

	std::cout << "Loading " + path + " ...\n";
	for (size_t i = 0; i < _generators[index].moveTable.capacity(); i++)
	{
		if (read(fd, &buf, sizeof(int)) == -1)
			throw std::runtime_error(std::string("Could not load ") + path);
		tables[index][i] = buf;
	}
	close(fd);
}

void
PruningTables::_generate(int index)
{
	const Generator&	gen(_generators[index]);
	std::string			filename("./tables/" + gen.name);
	std::vector<int>&	table = tables[index];
	
	int	fd = open(filename.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH);
	if (fd == -1)
		throw std::runtime_error(std::string("Could not generate ") + filename);

	std::list<int>		buffer(1, 0);
	std::list<int>		tmp;
	size_t				filled	= 0;
	size_t				depth	= 0;

	while (filled < table.size())
	{
		for (
			std::list<int>::const_iterator it = buffer.begin();
			it != buffer.end();
			it++
		) {
			if (table[*it] == -1)
			{
				table[*it] = depth;
				filled++;
			}

			for (int i = 0; i < MOVES_COUNT; i++)
			{
				int newCoord = gen.moveTable[*it][i];
				if (table[newCoord] == -1)
					tmp.push_back(newCoord);
			}
		}

		buffer = tmp;
		tmp.clear();
		depth++;
	}
	
	return ; 

	buffer.clear();
	for (size_t i = 1; i < table.size(); i++)
	{
		if (table[i] == -1)
			buffer.push_back(i);
	}

	while (buffer.size())
	{
		std::list<int>::const_iterator it = buffer.begin();
		
		while (it != buffer.end())
		{
			bool found = false;

			for (int i = 0; !found && i < MOVES_COUNT; i++)
			{
				int newCoord = gen.moveTable[*it][i];
				if (table[newCoord] >= 0)
				{
					found = true;
					table[*it] = table[newCoord] + 1;
					it = buffer.erase(it);
					filled++;
				}
			}

			if (!found)
				it++;
		}
	}

	for (size_t i = 0; i < table.size(); i++)
	{
		if (write(fd, &(table[i]), sizeof(int)) == -1)
			throw std::runtime_error(std::string("Could not generate ") + filename);
	}

	close(fd);
}
