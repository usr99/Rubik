/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BaseGenerator.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 18:50:28 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/30 18:54:52 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASEGENERATOR_HPP
# define BASEGENERATOR_HPP

# include "rubik.hpp"

using BaseGenerator = struct BaseGenerator {
	
	BaseGenerator(
		const std::string& name,
		size_t size
	);

	const std::string	name;
	size_t				size;
};

#endif
