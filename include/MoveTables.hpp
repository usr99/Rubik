/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MoveTables.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 22:15:41 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/02 16:15:54 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVETABLES_HPP
# define MOVETABLES_HPP

# include "ATable.hpp"
# include "CubieCube.hpp"

class MoveTables : public ATable<u_int16_t>
{
	public:

		enum MoveTable {
			CORNER_ORI, EDGE_ORI, UD_SLICE,
			CORNER_PERM, EDGE_P2, UD_SLICE_P2
		};

		static MoveTables*	getInstance();
		static void			destroy();
		virtual ~MoveTables();

	private:

		struct Generator;

		MoveTables();

		virtual void		_load(int index, int fd);
		virtual void		_generate(int index, int fd);

		static MoveTables*	_instance;
};

struct MoveTables::Generator : public BaseGenerator
{
	Generator(
		const std::string &name,
		size_t size,
		void (CubieCube::*set)(u_int16_t),
		u_int16_t (CubieCube::*get)(void) const
	);
	
	void (CubieCube::*set)(u_int16_t);
	u_int16_t (CubieCube::*get)(void) const;
};

#endif
