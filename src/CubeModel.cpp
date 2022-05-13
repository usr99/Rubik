/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubeModel.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 18:36:46 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/13 17:56:51 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Renderer.hpp"
#include "CubeModel.hpp"

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

std::array<glm::vec3, 6> CubeModel::ColorScheme = {
	glm::vec3(1.000f, 1.000f, 1.000f),	// Up:		White
	glm::vec3(0.478f, 0.000f, 0.000f),	// Right:	Red
	glm::vec3(0.098f, 0.412f, 0.094f),	// Front:	Green
	glm::vec3(1.000f, 0.835f, 0.000f),	// Down:	Yellow
	glm::vec3(0.722f, 0.314f, 0.000f),	// Left:	Orange
	glm::vec3(0.000f, 0.114f, 0.529f)	// Back:	Blue
};

CubeModel::CubeModel(Shader& shader, const FaceletCube& rhs)
	: AnimEnabled(true), Delay(1.0f),
		_FaceletTex("res/images/facelet.png"),
		_Updated(true),
		_Faces(new std::array<Face, 6>({
			Face( // UP
				rhs.getFacelets().data() + 0 * 9,
				glm::vec3(-1.0f, 0.0f, 0.0f),
				90.0f
			),
			Face( // RIGHT
				rhs.getFacelets().data() + 1 * 9,
				glm::vec3(0.0f, 1.0f, 0.0f),
				90.0f
			),
			Face( // FRONT
				rhs.getFacelets().data() + 2 * 9,
				glm::vec3(1.0f, 1.0f, 1.0f),
				0.0f
			),
			Face( // DOWN
				rhs.getFacelets().data() + 3 * 9,
				glm::vec3(1.0f, 0.0f, 0.0f),
				90.0f
			),
			Face( // LEFT
				rhs.getFacelets().data() + 4 * 9,
				glm::vec3(0.0f, -1.0f, 0.0f),
				90.0f
			),
			Face( // BACK
				rhs.getFacelets().data() + 5 * 9,
				glm::vec3(0.0f, 1.0f, 0.0f),
				180.0f
			)
		}))
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

	if (_WaitingMoves.size())
	{
		static std::clock_t last_clock = std::clock();
		std::clock_t diff = std::clock() - last_clock;
		
		if ((double)diff / (double)CLOCKS_PER_SEC > 0.01 / (double)Delay)
		{
			Faceturn& ft = _WaitingMoves.front();
			
			_TurnFace(ft);
			last_clock = std::clock();

			if (ft.currentAngle == ft.finalAngle)
				_WaitingMoves.pop_front();
		}
	}

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

CubeModel::Faceturn::Faceturn(int index, float angle)
: face(static_cast<Turn>(index)), finalAngle(angle), currentAngle(0.0f), clockwise(finalAngle >= 0.0) {}

void
CubeModel::PushMove(int index, float angle)
{
	if (!_WaitingMoves.size())
		_WaitingMoves.emplace_back(index, angle);
}

void
CubeModel::ApplySequence(const std::list<std::string>& seq)
{
	for (
		std::list<std::string>::const_iterator it = seq.begin();
		it != seq.end();
		it++
	) {
		try {
			const char singmaster = it->at(0);
			float angle = 90.0f;

			if (it->length() == 2)
			{
				if (it->at(1) == '2')
					angle = 180.0f; // half-turn
				else if (it->at(1) == '\'')
					angle = -90.0f; // counter clockwise
				else
					throw std::exception(); // not an existing move
			}
			else if (it->length() != 1)
				throw std::exception(); // not an existing move

			int idx = 0;
			while (Rubik::Faces[idx] != singmaster)
				idx++;
			_WaitingMoves.emplace_back(idx, angle);
		} catch (const std::exception& e) {
			_WaitingMoves.clear();
			throw std::invalid_argument("Scramble is not valid.");
		}
	}
}

bool
CompareCorners(int src, int facelets[3], int& orientation)
{
	for (int i = 0; i < 3; i++)
	{
		bool found = false;
		for (int j = 0; j < 3; j++ && !found)
		{
			if (Rubik::CornerFacelets[src][i] / 9 == facelets[j])
			{
				found = true;

				if (i == 0)
					orientation = j;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

bool
CompareEdges(int src, int facelets[2], int& orientation)
{
	for (int i = 0; i < 2; i++)
	{
		bool found = false;
		for (int j = 0; j < 2; j++ && !found)
		{
			if (Rubik::EdgeFacelets[src][i] / 9 == facelets[j])
			{
				found = true;

				if (i == 0)
					orientation = j;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

CubieCube
CubeModel::toCubieCube() const
{
	std::array<CornerCubie, CORNER_COUNT>	corners;
	std::array<EdgeCubie, EDGE_COUNT>		edges;

	for (int i = 0; i < CORNER_COUNT; i++)
	{
		const int ref[3] = {
			Rubik::CornerFacelets[i][0],
			Rubik::CornerFacelets[i][1],
			Rubik::CornerFacelets[i][2]
		};

		int facelets[3];
		for (int j = 0; j < 3; j++)
		{
			const int faceIdx = ref[j] / 9;
			const int faceletIdx = ref[j] % 9;

			int k = 0;
			while (_Faces->at(faceIdx).facelets[faceletIdx].color != ColorScheme[k])
				k++;
			facelets[j] = k;
		}

		int j = 0;
		int o = 0;
		while (!CompareCorners(j, facelets, o))
			j++;

		corners[i].c = static_cast<Corner>(j);
		corners[i].o = o;
	}

	for (int i = 0; i < EDGE_COUNT; i++)
	{
		const int ref[2] = {
			Rubik::EdgeFacelets[i][0],
			Rubik::EdgeFacelets[i][1]
		};

		int facelets[2];
		for (int j = 0; j < 2; j++)
		{
			const int faceIdx = ref[j] / 9;
			const int faceletIdx = ref[j] % 9;

			int k = 0;
			while (_Faces->at(faceIdx).facelets[faceletIdx].color != ColorScheme[k])
				k++;
			facelets[j] = k;
		}

		int j = 0;
		int o = 0;
		while (!CompareEdges(j, facelets, o))
			j++;

		edges[i].e = static_cast<Edge>(j);
		edges[i].o = o;
	}

	return CubieCube(corners, edges);
}

void
CubeModel::_TurnFace(Faceturn& ft)
{
	const Face::FaceTurnDesc& turnInfo = Face::RotationRules[ft.face];
	float stepAngle = AnimEnabled ? 2.0f : 90.0f;

	if (!ft.clockwise)
		stepAngle = -stepAngle;

	_RotateFaceInstances(ft.face, glm::rotate(glm::mat4(1.0f), glm::radians(stepAngle), turnInfo.RotationAxis));
	ft.currentAngle += stepAngle;

	if (fmod(ft.currentAngle, 90.0f) == 0)
		_RotateFaceData(ft.face, ft.clockwise);
	_Updated = true;
}

void
CubeModel::_UpdateInstances()
{
	if (!_Updated)
		return ; // no need to refresh the buffer if the cube hasn't changed

	/* Copy the new facelets data into the instance buffer */
	for (auto f = _Faces->begin(); f != _Faces->end(); f++)
		_FaceletInstances->update(f->offset, f->facelets.data(), 9 * sizeof(Instance));
	_Updated = false;
}

void
CubeModel::_RotateFaceInstances(Turn face, const glm::mat4& rotation)
{
	const Face::FaceTurnDesc& turnInfo = Face::RotationRules[face];

	for (auto f = _Faces->at(face).facelets.begin(); f != _Faces->at(face).facelets.end(); f++)
		f->transform = rotation * f->transform;
	for (auto f = turnInfo.AdjacentFaces.begin(); f != turnInfo.AdjacentFaces.end(); f++)
		_RotateRow(_Faces->at(f->face).rows[f->row], rotation);
}

void
CubeModel::_RotateFaceData(Turn face, bool clockwise)
{
	const Face::FaceTurnDesc&	turnInfo(Face::RotationRules[face]);
	std::array<Instance, 9>		copy(_Faces->at(face).facelets);

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (clockwise)
				_Faces->at(face).facelets[i * 3 + j] = copy[((2 - j) * 3) + i];
			else
				_Faces->at(face).facelets[((2 - j) * 3) + i] = copy[i * 3 + j];
		}
	}

	const Face::FaceRowDesc& start = turnInfo.AdjacentFaces[0];
	if (clockwise)
	{
		for (auto f = turnInfo.AdjacentFaces.begin() + 1; f != turnInfo.AdjacentFaces.end(); f++)
			_SwapFaceRows(_Faces->at(start.face).rows[start.row], _Faces->at(f->face).rows[f->row], f->reverse);
	}
	else
	{
		for (auto f = turnInfo.AdjacentFaces.rbegin(); f != turnInfo.AdjacentFaces.rend() - 1; f++)
			_SwapFaceRows(_Faces->at(start.face).rows[start.row], _Faces->at(f->face).rows[f->row], f->reverse);
	}
}

void
CubeModel::_RotateRow(std::array<Instance*, 3>& row, const glm::mat4& rotation)
{
	for (auto sq = row.begin(); sq != row.end(); sq++)
		(*sq)->transform = rotation * (*sq)->transform;
}

void
CubeModel::_SwapFaceRows(const std::array<Instance*, 3>& from, const std::array<Instance*, 3> to, bool reverse)
{
	for (unsigned int i = 0; i < 3; i++)
	{
		if (reverse)
			std::swap(*from[i], *to[2 - i]);
		else
			std::swap(*from[i], *to[i]);
	}
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
	Face::NextOffset += facelets.size() * sizeof(Instance);
}
