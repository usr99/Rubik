/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:13:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/30 00:18:14 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRUNINGTABLES_HPP
# define PRUNINGTABLES_HPP

# include "MoveTables.hpp"

class PruningTables : public ATable<int>
{
	public:

		static PruningTables*	getInstance(const MoveTables& mt);
		virtual ~PruningTables();

	private:

		struct Generator;

		PruningTables(const MoveTables& mt);

		virtual void			_load(int index, int fd);
		virtual void			_generate(int index, int fd);

		static PruningTables*	_instance;
};

struct PruningTables::Generator : public BaseGenerator
{
	Generator(
		const std::string& name,
		const std::vector<Move>& allowedMoves,
		size_t max,
		const std::vector<std::vector<int>>& mt
	);
	
	const std::vector<Move>&				allowedMoves;
	const std::vector<std::vector<int>>&	moveTable;
};

#endif
