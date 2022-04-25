/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MoveTables.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 22:15:41 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/25 01:13:50 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVETABLES_HPP
# define MOVETABLES_HPP

# include "CubieCube.hpp"

# define MOVE_TABLES_COUNT 3

class MoveTables
{
	public:

		enum Table { CORNER_ORI, EDGE_ORI, UD_SLICE };

		MoveTables();
		
		std::vector<std::vector<long>>	tables[MOVE_TABLES_COUNT];

	private:

		using Generator = struct Generator {

			Generator(
				const std::string& name,
				void (CubieCube::*set)(long),
				long (CubieCube::*get)(void) const,
				int max
			);

			const std::string& name;
			void (CubieCube::*set)(long);
			long (CubieCube::*get)(void) const;
			int max;
		};

		void _load(int index);
		void _generate(int index);

		const std::vector<Generator> _generators;
};

#endif
