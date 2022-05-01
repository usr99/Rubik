/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MoveTables.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 22:15:41 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/30 18:55:31 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MOVETABLES_HPP
# define MOVETABLES_HPP

# include "ATable.hpp"
# include "CubieCube.hpp"

class MoveTables : public ATable<std::vector<int>>
{
	public:

		static MoveTables*	getInstance();
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
		void (CubieCube::*set)(int),
		int (CubieCube::*get)(void) const
	);
	
	void (CubieCube::*set)(int);
	int (CubieCube::*get)(void) const;
};

#endif
