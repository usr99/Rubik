/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cube.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 17:41:54 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/15 03:04:43 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_HPP
# define CUBE_HPP

# include <map>
# include <vector>
# include <list>

# include "../include/Color.hpp"
# include "../include/rubik.hpp"

class Cube
{
	public:
	
		Cube(const std::list<std::string>& scramble);

		void	moveLeft();
		void	moveRight();
		void	moveUp();
		void	moveDown();
		void	moveFront();
		void	moveBack();

		void	render();

		std::vector<std::vector<int>> cubies;

	private:

		Cube();

		void	_rotateFace(std::vector<int>& face);
		void	_swapVerticalRow(std::vector<int>& face, std::vector<int>& row, bool isRightRow);
		void	_swapHorizontalRow(std::vector<int>& face, std::vector<int>& row, bool isUpRow);

		using MoveFunctionPtr = void (Cube::*)(void);
		
		static const std::map<char, MoveFunctionPtr> _notationReference;
};

#endif
