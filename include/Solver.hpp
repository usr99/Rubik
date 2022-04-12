/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/09 02:11:36 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/11 23:00:30 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_HPP
# define SOLVER_HPP

# include <vector>
# include "rubik.hpp"
# include "Color.hpp"

class Solver {

	public:

		Solver(); // constructor

		void	render();
		void	moveLeft();
		void	moveRight();
		void	moveUp();
		void	moveDown();
		void	moveFront();
		void	moveBack();

	private:

		void	_rotateFace(std::vector<int>& face);

		std::vector<std::vector<int>>	_cubies;
};

#endif
