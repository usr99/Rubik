/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cube.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 18:39:09 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/15 03:19:21 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include "../include/Cube.hpp"

const std::map<char, Cube::MoveFunctionPtr>	Cube::_notationReference({
	std::make_pair('R', &Cube::moveRight),
	std::make_pair('L', &Cube::moveLeft),
	std::make_pair('U', &Cube::moveUp),
	std::make_pair('D', &Cube::moveDown),
	std::make_pair('F', &Cube::moveFront),
	std::make_pair('B', &Cube::moveBack)
});

Cube::Cube() {} // default constructor (private)

Cube::Cube(const std::list<std::string>& scramble)
{
	// set the cube in a solved state
	for (Color::iterator it = Color::begin(); it != Color::end(); ++it)
		this->cubies.push_back(std::vector<int>(9, *it));

	for ( // for each instruction of the scramble
		std::list<std::string>::const_iterator it = scramble.begin();
		it != scramble.end();
		it++)
	{
		try {
			MoveFunctionPtr	fn		= _notationReference.at(it->front());
			int 			factor	= 1;

			if (it->length() == 2)
			{
				if (it->at(1) == '2')
					factor = 2; // half turn moves
				else if (it->at(1) == '\'')
					factor = 3; // counter clockwise moves
				else
					throw std::exception(); // not an existing move
			}
			else if (it->length() != 1)
				throw std::exception(); // not an existing move

			for (int i = 0; i < factor; i++)
				(this->*fn)(); // execute the move

		} catch (const std::exception& e) {
			throw std::invalid_argument("Scramble is not valid.");
		}
	}
}

void
Cube::moveRight()
{
	std::vector<int>	row({
		this->cubies[FRONT][2],
		this->cubies[FRONT][5],
		this->cubies[FRONT][8]
	}); // copy affected row on the front face

	_rotateFace(this->cubies[RIGHT]);

	// swap a row with all adjacent faces in clockwise order
	_swapVerticalRow(this->cubies[UP], row, true);
	std::reverse(row.begin(), row.end());
	_swapVerticalRow(this->cubies[BACK], row, false);
	std::reverse(row.begin(), row.end());
	_swapVerticalRow(this->cubies[DOWN], row, true);
	_swapVerticalRow(this->cubies[FRONT], row, true);
}

void
Cube::moveLeft()
{
	std::vector<int>	row({
		this->cubies[FRONT][0],
		this->cubies[FRONT][3],
		this->cubies[FRONT][6]
	});

	_rotateFace(this->cubies[LEFT]);
	_swapVerticalRow(this->cubies[DOWN], row, false);
	std::reverse(row.begin(), row.end());
	_swapVerticalRow(this->cubies[BACK], row, true);
	std::reverse(row.begin(), row.end());
	_swapVerticalRow(this->cubies[UP], row, false);
	_swapVerticalRow(this->cubies[FRONT], row, false); 
}

void
Cube::moveUp()
{
	std::vector<int>	row({
		this->cubies[FRONT][0],
		this->cubies[FRONT][1],
		this->cubies[FRONT][2]
	});

	_rotateFace(this->cubies[UP]);
	_swapHorizontalRow(this->cubies[LEFT], row, false);
	_swapHorizontalRow(this->cubies[BACK], row, false);
	_swapHorizontalRow(this->cubies[RIGHT], row, false);
	_swapHorizontalRow(this->cubies[FRONT], row, false);
}

void
Cube::moveDown()
{
	std::vector<int>	row({
		this->cubies[FRONT][6],
		this->cubies[FRONT][7],
		this->cubies[FRONT][8]
	});

	_rotateFace(this->cubies[DOWN]);
	_swapHorizontalRow(this->cubies[RIGHT], row, true); 
	_swapHorizontalRow(this->cubies[BACK], row, true);
	_swapHorizontalRow(this->cubies[LEFT], row, true);
	_swapHorizontalRow(this->cubies[FRONT], row, true);
}

void
Cube::moveFront()
{
	std::vector<int>	row({
		this->cubies[UP][6],
		this->cubies[UP][7],
		this->cubies[UP][8]
	});

	_rotateFace(this->cubies[FRONT]);
	_swapVerticalRow(this->cubies[RIGHT], row, false); 
	std::reverse(row.begin(), row.end());
	_swapHorizontalRow(this->cubies[DOWN], row, false);
	_swapVerticalRow(this->cubies[LEFT], row, true);
	std::reverse(row.begin(), row.end());
	_swapHorizontalRow(this->cubies[UP], row, true);
}

void
Cube::moveBack()
{
	std::vector<int>	row({
		this->cubies[UP][0],
		this->cubies[UP][1],
		this->cubies[UP][2]
	});

	_rotateFace(this->cubies[BACK]);
	std::reverse(row.begin(), row.end());
	_swapVerticalRow(this->cubies[LEFT], row, false); 
	_swapHorizontalRow(this->cubies[DOWN], row, true);
	std::reverse(row.begin(), row.end());
	_swapVerticalRow(this->cubies[RIGHT], row, true);
	_swapHorizontalRow(this->cubies[UP], row, false);
}

/*
** Perform a clockwise rotation on the face
**
**	0 1 2		6 3 0
**	3 4 5	=>	7 4 1
**	6 7 8		8 5 2
*/
void
Cube::_rotateFace(std::vector<int>& face)
{
	std::vector<int> copy(face);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			face[i * 3 + j] = copy[((2 - j) * 3) + i];
	}
}

/*
** Swap the cubies from the "row" argument with those from a vertical row of the specified face
*/
void
Cube::_swapVerticalRow(std::vector<int>& face, std::vector<int>& row, bool isRightRow)
{
	// first cubie's index for the LEFT column is 0
	// but 2 for the RIGHT column
	int cubieIndex = isRightRow * 2;

	for (int i = 0; i < 3; i++)
	{
		std::swap(face[cubieIndex], row[i]);
		cubieIndex += 3; // move down one line for the next cubie
	}
}

/*
** Same but horizontally
*/
void
Cube::_swapHorizontalRow(std::vector<int>& face, std::vector<int>& row, bool isUpRow)
{
	// first cubie's index for the UP row is 0
	// but 2 for the DOWN row
	const int rowIndex = isUpRow * 2;

	for (int i = 0; i < 3; i++)
	{
		const int cubieIndex = rowIndex * 3 + i;
		std::swap(face[cubieIndex], row[i]);
	}
}
