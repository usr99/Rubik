/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 17:53:09 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/20 23:35:48 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rubik.hpp"

unsigned int factorial(unsigned int n)
{
	if (n == 0 || n == 1)
		return 1;
	else
		return (n * factorial(n - 1));
}

unsigned int binomial(unsigned int n, unsigned int k)
{
	if (k > n)
		throw std::invalid_argument("k must be less than n");
	return factorial(n) / (factorial(k) * factorial(n - k));
}
