/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATable.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 17:42:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 00:52:11 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TABLE_HPP
# define TABLE_HPP

# include <sys/stat.h>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>

# include "BaseGenerator.hpp"

template <class T>
class ATable
{
	public:

		ATable(const std::vector<BaseGenerator*>& gen);
		const std::vector<std::vector<T>>& operator[](size_t n) const;

		virtual ~ATable();

	protected:

		void			_create(int index);
		virtual void	_load(int index, int fd) = 0;
		virtual void	_generate(int index, int fd) = 0;

		std::vector<std::vector<std::vector<T>>>	_tables;
		std::vector<BaseGenerator*>					_generators;

	private:

		ATable();
		ATable(const ATable& rhs);
		ATable& operator=(const ATable& rhs);
};

# include "ATable.ipp"

#endif
