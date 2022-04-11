/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 17:03:58 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/11 20:49:13 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rubik.hpp"
#include "../include/Solver.hpp"

int main(int ac, char **av)
{
	try
	{
		if (ac < 2)
			throw std::invalid_argument("A scramble is needed");

		Solver cube;
		cube.render();

		return (0);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl; 
		return (1);
	}
}
