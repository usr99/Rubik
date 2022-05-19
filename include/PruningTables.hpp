/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PruningTables.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 22:13:26 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/19 18:09:43 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRUNINGTABLES_HPP
# define PRUNINGTABLES_HPP

# include "MoveTables.hpp"

typedef std::pair<u_int16_t, u_int16_t> coordinates_t;

class PruningTables : public ATable<int8_t>
{
	public:

		static PruningTables*	getInstance(const MoveTables& mt, LoadingInfo* info = nullptr);
		static void				destroy();
		virtual ~PruningTables();

	private:

		struct Generator;

		PruningTables(const MoveTables& mt, LoadingInfo* info);

		virtual void			_load(int index, FILE* file);
		virtual void			_generate(int index, FILE* file);

		static PruningTables*	_instance;

		const MoveTables&		_mt;
};

struct PruningTables::Generator : public BaseGenerator
{
	Generator(
		const std::string& name,
		size_t size,
		const std::vector<Move>& allowedMoves,
		unsigned int mt1,
		unsigned int mt2
	);
	
	const std::vector<Move>&	allowedMoves;
	unsigned int				mtIndex1;
	unsigned int				mtIndex2;
};

#endif
