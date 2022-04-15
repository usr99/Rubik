/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 02:11:36 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/15 03:19:14 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_HPP
# define SOLVER_HPP

# include "../include/Cube.hpp"

class Solver {

	public:

		Solver(const std::list<std::string>& scramble); // constructor

		void	render();

	private:

		Solver();

		Cube	_cube;
};

#endif
