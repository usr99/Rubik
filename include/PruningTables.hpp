/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:13:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/27 22:28:11 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRUNINGTABLES_HPP
# define PRUNINGTABLES_HPP

# include "MoveTables.hpp"

class PruningTables
{
	public:

		PruningTables(const MoveTables& mt);

		std::vector<int>	tables[TABLES_COUNT];

	private:

		using Generator = struct Generator {

			Generator( // constructor
				const std::string& name,
				const std::vector<std::vector<int>>& mt
			);

			const std::string						name;
			const std::vector<std::vector<int>>&	moveTable;
		};

		void _load(int index);
		void _generate(int index);

		const std::vector<Generator> _generators;
};

#endif
