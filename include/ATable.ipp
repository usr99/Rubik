/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATable.ipp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 18:19:13 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/19 18:11:34 by mamartin         ###   ########.fr       */
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
ATable<T>::_create(int index, LoadingInfo* info)
{
	const std::string	path("./tables/" + _generators[index]->name);
	struct stat			st;
	FILE*				file;
	
	// create table file if it doesn't already exist
	if (stat(path.c_str(), &st) == -1)
	{
		if (errno == ENOENT) // table not found
		{
			try {

				file = fopen(path.c_str(), "w");
				if (!file)
					throw std::exception();

				if (info)
				{
					pthread_mutex_lock(&info->mutex);
					info->message = "Generating " + path + " ...";
					pthread_mutex_unlock(&info->mutex);
				}
				_generate(index, file); // call sub-class generation function
			} catch (const std::exception& e) {
				fclose(file);
				remove(path.c_str()); // remove file to force regeneration next time
				throw std::runtime_error("Could not generate " + _generators[index]->name);
			}
			fclose(file);
			return ;
		}
		else {
			throw std::runtime_error("Could not open " + _generators[index]->name);
		}
	}

	try { // load data from the table

		file = fopen(path.c_str(), "r");
		if (!file)
			throw std::exception();

		if (info)
		{
			pthread_mutex_lock(&info->mutex);
			info->message = "Loading " + path + " ...";
			pthread_mutex_unlock(&info->mutex);
		}
		_load(index, file); // call sub-class loading function
	} catch (const std::exception& e) {
		fclose(file);
		remove(path.c_str()); // remove file to force regeneration next time
		throw std::runtime_error(std::string("Could not load ") + path);
	}
	fclose(file);
}
