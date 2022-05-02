/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATable.ipp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 18:19:13 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 02:26:58 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ATable.hpp"

template <class T>
ATable<T>::ATable(const std::vector<BaseGenerator*>& gen) :
	_tables(gen.size()), _generators(gen)
{
	struct stat	st;

	// create ./tables folder if it doesn't already exist
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
}

template <class T>
const std::vector<std::vector<T>>&
ATable<T>::operator[](size_t n) const
{
	return _tables[n];
}

template <class T>
ATable<T>::~ATable()
{
	for ( // delete generators
		std::vector<BaseGenerator*>::const_iterator it = _generators.begin();
		it != _generators.end();
		it++
	)	delete *it;
}

/*
** This function generates or loads any table, whether it's a MoveTable or a PruningTable
** only handles existence check, file opening and file creation
** generation and loading routines are defined in sub-classes
*/
template <class T>
void
ATable<T>::_create(int index)
{
	const std::string	path("./tables/" + _generators[index]->name);
	struct stat			st;
	int					fd;
	
	// create table file if it doesn't already exist
	if (stat(path.c_str(), &st) == -1)
	{
		if (errno == ENOENT) // table not found
		{
			try {
				if ((fd = open(path.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IROTH)) == -1)
					throw std::exception();
				std::cout << "Generating " + path + " ...\n";
				_generate(index, fd); // call sub-class generation function
			} catch (const std::exception& e) {
				close(fd);
				remove(path.c_str()); // remove file to force regeneration next time
				throw std::runtime_error("Could not generate " + _generators[index]->name);
			}
			close(fd);
			return ;
		}
		else {
			throw std::runtime_error("Could not open " + _generators[index]->name);
		}
	}

	try { // load data from the table
		if ((fd = open(path.c_str(), O_RDONLY)) == -1)
			throw std::exception();
		std::cout << "Loading " + path + " ...\n";
		_load(index, fd); // call sub-class loading function
	} catch (const std::exception& e) {
		close(fd);
		remove(path.c_str()); // remove file to force regeneration next time
		throw std::runtime_error(std::string("Could not load ") + path);
	}
	close(fd);
}
