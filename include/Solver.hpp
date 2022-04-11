/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 02:11:36 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/11 20:45:29 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_HPP
# define SOLVER_HPP

# include "rubik.hpp"
# include "Color.hpp"

class Solver {

	public:

		Solver(); // constructor

		void	render();
		void	moveLeft(int factor);
		void	moveRight(int factor);
		void	moveUp(int factor);
		void	moveDown(int factor);
		void	moveFront(int factor);
		void	moveBack(int factor);

	private:

		int	_cubies[6][9];
};

#endif
