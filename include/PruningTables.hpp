/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:13:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/01 00:29:53 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRUNINGTABLES_HPP
# define PRUNINGTABLES_HPP

# include "MoveTables.hpp"

typedef int8_t							pruning_t;
typedef std::pair<u_int16_t, u_int16_t> coordinates_t;

class PruningTables : public ATable<std::vector<pruning_t>>
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
		const std::vector<std::vector<int>>& mt1,
		const std::vector<std::vector<int>>& mt2
	);
	
	const std::vector<Move>&				allowedMoves;
	const std::vector<std::vector<int>>&	moveTable1;
	const std::vector<std::vector<int>>&	moveTable2;
};

#endif
