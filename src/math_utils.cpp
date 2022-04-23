/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/23 17:53:09 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/23 18:11:30 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/rubik.hpp"

unsigned long factorial(unsigned int n)
{
	if (n == 0 || n == 1)
		return 1;
	else
		return (n * factorial(n - 1));
}

unsigned long binomial(unsigned int n, unsigned int k)
{
	if (k > n)
		throw std::invalid_argument("k must be less than n");
	return factorial(n) / (factorial(k) * factorial(n - k));
}
