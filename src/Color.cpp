/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/11 20:26:11 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/12 10:04:15 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Color.hpp"

const int Color::WHITE		= 231;
const int Color::ORANGE		= 202;
const int Color::GREEN		= 28;
const int Color::RED		= 160;
const int Color::BLUE		= 20;
const int Color::YELLOW		= 11;

const std::list<int> Color::_All = {
	Color::WHITE,
	Color::ORANGE,
	Color::GREEN,
	Color::RED,
	Color::BLUE,
	Color::YELLOW
};

Color::iterator Color::begin()
{
	return _All.begin();
}

Color::iterator Color::end()
{
	return _All.end();
}
