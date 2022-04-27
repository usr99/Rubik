/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MoveTables.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 22:15:41 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/27 22:29:08 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVETABLES_HPP
# define MOVETABLES_HPP

# include <sys/stat.h>
# include <cstring>
# include <fcntl.h>
# include <unistd.h>

# include "CubieCube.hpp"

class MoveTables
{
	public:

		MoveTables();
		
		std::vector<std::vector<int>>	tables[TABLES_COUNT];

	private:

		using Generator = struct Generator {

			Generator( // constructor
				const std::string& name,
				void (CubieCube::*set)(int),
				int (CubieCube::*get)(void) const,
				int max
			);

			const std::string name;
			void (CubieCube::*set)(int);
			int (CubieCube::*get)(void) const;
			int max;
		};

		void _load(int index);
		void _generate(int index);

		const std::vector<Generator> _generators;
};

#endif
