/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubemodel_utils.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/13 21:48:07 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/13 23:32:54 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cubemodel_utils.hpp"

const std::array<CubeModel::Face::FaceTurnDesc, 6> CubeModel::Face::RotationRules = {
	{{	// UP
		.AdjacentFaces = {{
			{ .face = F,	.row = Top,		.reverse = false },
			{ .face = L,	.row = Top,		.reverse = false },
			{ .face = B,	.row = Top,		.reverse = false },
			{ .face = R,	.row = Top,		.reverse = false }
		}},
		.RotationAxis = { 0.0f, -1.0f, 0.0f },
	},
	{	// RIGHT
		.AdjacentFaces = {{
			{ .face = U,	.row = Right,	.reverse = false },
			{ .face = B,	.row = Left,	.reverse = true },
			{ .face = D,	.row = Right,	.reverse = false },
			{ .face = F,	.row = Right,	.reverse = false }
		}},
		.RotationAxis = { -1.0f, 0.0f, 0.0f },
	},
	{	// FRONT
		.AdjacentFaces = {{
			{ .face = U,	.row = Bottom,	.reverse = false },
			{ .face = R,	.row = Left,	.reverse = false },
			{ .face = D,	.row = Top,		.reverse = true },
			{ .face = L,	.row = Right,	.reverse = true }
		}},
		.RotationAxis = { 0.0f, 0.0f, -1.0f },
	},
	{	// DOWN
		.AdjacentFaces = {{
			{ .face = F,	.row = Bottom,	.reverse = false },
			{ .face = R,	.row = Bottom,	.reverse = false },
			{ .face = B,	.row = Bottom,	.reverse = false },
			{ .face = L,	.row = Bottom,	.reverse = false }
		}},
		.RotationAxis = { 0.0f, 1.0f, 0.0f },
	},
	{	// LEFT
		.AdjacentFaces = {{
			{ .face = U,	.row = Left,	.reverse = false },
			{ .face = F,	.row = Left,	.reverse = false },
			{ .face = D,	.row = Left,	.reverse = false },
			{ .face = B,	.row = Right,	.reverse = true }
		}},
		.RotationAxis = { 1.0f, 0.0f, 0.0f },
	},
	{	// BACK
		.AdjacentFaces = {{
			{ .face = U,	.row = Top,		.reverse = false },
			{ .face = L,	.row = Left,	.reverse = true },
			{ .face = D,	.row = Bottom,	.reverse = true },
			{ .face = R,	.row = Right,	.reverse = false }
		}},
		.RotationAxis = { 0.0f, 0.0f, 1.0f },
	}}
};

CubeModel::Instance::Instance()
	: color(nullptr), transform(1.0f) {}

void
CubeModel::Instance::init(
	glm::vec3* color,
	const glm::vec3& rot,
	glm::vec3 translation,
	float angle
) {
	/* Create the transformation matrix from the translation and rotation arguments */
	if (angle)
	{
		translation	= glm::rotate(translation, glm::radians(angle), rot);
		transform	= glm::translate(transform, translation);
		transform	= glm::rotate(transform, glm::radians(angle), rot);
	}
	else
		transform = glm::translate(transform, translation);
	this->color = color;
}

unsigned int CubeModel::Face::NextOffset = 0;
CubeModel::Face::Face(const Facelet* source, const glm::vec3& rotation, float angle)
{
	glm::vec3 pos(-0.30f, 0.30f, 0.45f);
	int idx = 0;

	/*
	** Initialize 9 instances of the same color and placed in a way to create a 3x3 grid
	** starting from the upper-left facelet
	*/
	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 3; x++)
		{
			facelets[idx].init(CubeModel::ColorScheme.data() + (*source / 9), rotation, pos, angle);

			/*
			** Save pointer to the current facelet if it belongs to a row
			** corners belong to one vertical and one horizontal rows
			** edges belong to one row, vertical or horizontal
			** centers are not part of any row
			*/
			if (x != 1) // check for a vertical row
				rows[x][y] = facelets.data() + idx;
			if (y != 1) // check for a horizontal row
				rows[y + 1][x] = facelets.data() + idx;

			idx++;
			pos.x += 0.30f;
			source++;
		}
		pos.x = -0.30f;
		pos.y -= 0.30f;
	}

	/* Keep the offset from where this face's data begins in the instance buffer */
	offset = Face::NextOffset;
	Face::NextOffset += facelets.size() * (sizeof(glm::vec3) + sizeof(glm::mat4));
}
