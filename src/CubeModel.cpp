/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubeModel.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 18:36:46 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/12 19:24:36 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Renderer.hpp"
#include "CubeModel.hpp"

std::array<glm::vec3, 6> CubeModel::ColorScheme = {
	glm::vec3(1.000f, 1.000f, 1.000f),	// Up:		White
	glm::vec3(0.478f, 0.000f, 0.000f),	// Right:	Red
	glm::vec3(0.098f, 0.412f, 0.094f),	// Front:	Green
	glm::vec3(1.000f, 0.835f, 0.000f),	// Down:	Yellow
	glm::vec3(0.722f, 0.314f, 0.000f),	// Left:	Orange
	glm::vec3(0.000f, 0.114f, 0.529f)	// Back:	Blue
};

CubeModel::CubeModel(Shader& shader, const FaceletCube& rhs)
	: _FaceletTex("res/images/rubik.png"),
		_Updated(true),
		_Faces({
			Face(
				rhs.getFacelets().data() + 0 * 9,
				glm::vec3(-1.0f, 0.0f, 0.0f),
				90.0f
			),
			Face(
				rhs.getFacelets().data() + 1 * 9,
				glm::vec3(0.0f, 1.0f, 0.0f),
				90.0f
			),
			Face(
				rhs.getFacelets().data() + 2 * 9,
				glm::vec3(1.0f, 1.0f, 1.0f),
				0.0f
			),
			Face(
				rhs.getFacelets().data() + 3 * 9,
				glm::vec3(1.0f, 0.0f, 0.0f),
				90.0f
			),
			Face(
				rhs.getFacelets().data() + 4 * 9,
				glm::vec3(0.0f, -1.0f, 0.0f),
				90.0f
			),
			Face(
				rhs.getFacelets().data() + 5 * 9,
				glm::vec3(0.0f, 1.0f, 0.0f),
				180.0f
			)
		})
{
	const Vertex square[] = {
		{ glm::vec3(-0.15f,  0.15f, 0.0f), glm::vec2(0.0f, 1.0f) }, // upper-left
		{ glm::vec3( 0.15f,  0.15f, 0.0f), glm::vec2(1.0f, 1.0f) }, // upper-right
		{ glm::vec3(-0.15f, -0.15f, 0.0f), glm::vec2(0.0f, 0.0f) }, // bottom-left
		{ glm::vec3( 0.15f, -0.15f, 0.0f), glm::vec2(1.0f, 0.0f) }	 // bottom-right
	};

	const unsigned int indices[] = {
		0, 3, 2, 0, 3, 1
	};

	/* Create the VertexBuffer describing one unique facelet */
	VertexBufferLayout VerticesLayout;
	_FaceletVertices = std::make_unique<VertexBuffer>(square, 4 * sizeof(Vertex));
	VerticesLayout.push<float>(3); // position
	VerticesLayout.push<float>(2); // texture coordinates
	_VAO.addBuffer(*_FaceletVertices, VerticesLayout, 0, false);

	/* Create the buffer describing all facelet instances */
	VertexBufferLayout InstancesLayout;
	_FaceletInstances = std::make_unique<VertexBuffer>(nullptr, 6 * 9 * sizeof(Instance));
	InstancesLayout.push<float>(3);		// color
	InstancesLayout.push<glm::mat4>(1);	// transformation matrix
	_VAO.addBuffer(*_FaceletInstances, InstancesLayout, 2, true);

	_FaceletIndices = std::make_unique<IndexBuffer>(indices, 6);

	/* Set the texture used for the facelet borders */
	shader.bind();
	shader.setUniform1i("u_Texture", 0);
}

void
CubeModel::Render()
{
	_VAO.bind();
	_FaceletTex.bind();
	_FaceletIndices->bind();

	_UpdateInstances();

	/* Draw 54 instances of the facelet */
	GLCall(glDrawElementsInstanced(
		GL_TRIANGLES,
		_FaceletIndices->getCount(),
		GL_UNSIGNED_INT,
		nullptr,
		6 * 9
	));
}

void
CubeModel::_UpdateInstances()
{
	if (!_Updated)
		return ; // no need to refresh the buffer if the cube hasn't changed

	/* Copy the new facelets data into the instance buffer */
	for (auto f = _Faces.begin(); f != _Faces.end(); f++)
		_FaceletInstances->update(f->offset, f->facelets.data(), 9 * sizeof(Instance));
	_Updated = false;
}

CubeModel::Instance::Instance()
	: color(0.0f, 0.0f, 0.0f), transform(1.0f) {}

void
CubeModel::Instance::init(
	const glm::vec3& color,
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
			facelets[idx].init(CubeModel::ColorScheme[*source / 9], rotation, pos, angle);

			/*
			** Save pointer to the current facelet if it belongs to a row
			** corners belong to one vertical and one horizontal rows
			** edges belong to one row, vertical or horizontal
			** centers are not part of any row
			*/
			if (x != 1) // check for a horizontal row
				rows[x][y] = facelets.data() + idx;
			if (y != 1) // check for a vertical row
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
	Face::NextOffset += facelets.size() * sizeof(Instance);
}
