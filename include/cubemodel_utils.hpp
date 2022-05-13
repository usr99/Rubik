/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubemodel_utils.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 21:46:09 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/13 23:51:55 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBEMODEL_UTILS_HPP
# define CUBEMODEL_UTILS_HPP

# include "CubeModel.hpp"

struct CubeModel::Face
{
	enum Row {
		Left = 0, Right	 = 2,
		Top	 = 1, Bottom = 3
	};

	struct FaceRowDesc
	{
		Turn face;		// which face is affected by the move ?
		Row	 row;		// which row of this face ?
		bool reverse;	// is it necessary to reverse the row before swapping its content ?
	};

	struct FaceTurnDesc
	{
		std::array<FaceRowDesc, 4>	AdjacentFaces;
		glm::vec3					RotationAxis;
	};

	Face(const Facelet* source, const glm::vec3& rotation, float angle);

	std::array<Instance, 9>					facelets;
	std::array<std::array<Instance*, 3>, 4>	rows;
	unsigned int							offset;

	static unsigned int NextOffset;
	static const std::array<FaceTurnDesc, 6> RotationRules;
};

#endif
