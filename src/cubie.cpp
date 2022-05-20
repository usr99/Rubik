/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubie.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/21 16:02:36 by user42            #+#    #+#             */
/*   Updated: 2022/05/20 23:35:48 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubie.hpp"

CornerCubie::CornerCubie() {}
CornerCubie::CornerCubie(const Corner c, const char o) :
	c(c), o(o) {}

Corner
CornerCubie::get() const { return c; }

EdgeCubie::EdgeCubie() {}
EdgeCubie::EdgeCubie(const Edge e, const char o) :
	e(e), o(o) {}

Edge
EdgeCubie::get() const { return e; }
