/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseGenerator.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 18:52:34 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/30 18:55:24 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/BaseGenerator.hpp"

BaseGenerator::BaseGenerator(
	const std::string& name,
	size_t size
) : name(name), size(size) {}
