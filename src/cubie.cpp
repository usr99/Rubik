/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubie.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 16:02:36 by user42            #+#    #+#             */
/*   Updated: 2022/04/23 02:01:03 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cubie.hpp"

CornerCubie::CornerCubie() {}
CornerCubie::CornerCubie(const Corner c, const char o) :
	c(c), o(o) {}

EdgeCubie::EdgeCubie() {}
EdgeCubie::EdgeCubie(const Edge e, const char o) :
	e(e), o(o) {}
