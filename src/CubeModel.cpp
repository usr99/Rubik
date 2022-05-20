/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubeModel.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 18:36:46 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/20 23:38:39 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Renderer.hpp"
#include "CubeModel.hpp"
#include "cubemodel_utils.hpp"
#include <glm/gtx/matrix_decompose.hpp>

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
		_42IconTex("res/images/42.png"),
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
		{ glm::vec3( 0.15f, -0.15f, 0.0f), glm::vec2(1.0f, 0.0f) }	// bottom-right
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
	_FaceletInstances = std::make_unique<VertexBuffer>(nullptr, (6 * 9 + 2) * sizeof(InstanceData));
	InstancesLayout.push<float>(3);		// color
	InstancesLayout.push<glm::mat4>(1);	// transformation matrix
	_VAO.addBuffer(*_FaceletInstances, InstancesLayout, 2, true);

	_FaceletIndices = std::make_unique<IndexBuffer>(indices, 6);

	/* Set the texture used for the facelet borders */
	shader.bind();
	_FaceletTex.bind(0);
	shader.setUniform1i("u_Texture", 0);
	_42IconTex.bind(1);
	shader.setUniform1i("u_LogoTexture", 1);
}

CubeModel::~CubeModel() { delete _Faces; }

void
CubeModel::Render()
{
	static double time;

	_VAO.bind();
	_FaceletTex.bind(0);
	_42IconTex.bind(1);
	_FaceletIndices->bind();

	/* Handle moves and animations */
	if (_WaitingMoves.size())
	{
		static std::clock_t last_clock = std::clock();
		std::clock_t diff = std::clock() - last_clock;
		
		if ((double)diff / (double)CLOCKS_PER_SEC > 0.01 / (double)Delay)
		{
			Faceturn& ft = _WaitingMoves.front(); // get first move in queue

			/* Before starting the rotation, create the black faces that will hide the inside of the cube */
			if (ft.currentAngle == 0.0f)
			{
				if (AnimEnabled)
					time = glfwGetTime();
				_CreateBlackFaces(ft);
			}

			_TurnFace(ft, time); // apply the move
			_UpdateBlackFaceInstance(0); // update the black face that rotates along the move

			if (ft.currentAngle == ft.finalAngle)
				_WaitingMoves.pop_front(); // the turn is complete, move is deleted
			last_clock = std::clock();
		}
	}

	/* Copy all instances into the instance buffer */
	for (auto f = _Faces->begin(); f != _Faces->end(); f++)
	{
		std::array<InstanceData, 9> tmp;
		for (unsigned int i = 0; i < 9; i++)
			tmp[i].init(f->facelets[i]);
		_FaceletInstances->update(f->offset, tmp.data(), 9 * sizeof(InstanceData));
	}

	/* Draw 54 instances of the facelet and 2 black faces */
	GLCall(glDrawElementsInstanced(
		GL_TRIANGLES,
		_FaceletIndices->getCount(),
		GL_UNSIGNED_INT,
		nullptr,
		6 * 9 + 2
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
	std::list<Faceturn> newMoves;

	for (
		std::list<std::string>::const_iterator it = seq.begin();
		it != seq.end();
		it++
	) {
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
		newMoves.emplace_back(idx, angle);
	}
	_WaitingMoves.insert(_WaitingMoves.end(), newMoves.begin(), newMoves.end());
}

FaceletCube
CubeModel::toFaceletCube() const
{
	std::array<Facelet, 54> faceletsArray;

	int index = 0;
	for (int i = 0; i < 6; i++)
	{
		for (auto f = _Faces->at(i).facelets.begin(); f != _Faces->at(i).facelets.end(); f++)
		{
			faceletsArray[index] = static_cast<Facelet>((f->color - ColorScheme.data()) * 9);
			index++;
		}
	}
	return FaceletCube(faceletsArray);
}

CubieCube
CubeModel::toCubieCube() const
{
	auto cube = toFaceletCube();

	return CubieCube(
		cube.getCornerCubies(),
		cube.getEdgeCubies()
	);
}

void
CubeModel::setState(const FaceletCube& from)
{
	auto reference = from.getFacelets().begin();

	for (int i = 0; i < 6; i++)
	{
		for (auto facelet = _Faces->at(i).facelets.begin(); facelet != _Faces->at(i).facelets.end(); facelet++)
		{
			facelet->color = ColorScheme.data() + *reference / 9;
			reference++;
		}
	}

	/* Reset the up face center facelet so that the 42 sticker is well oriented */
	_Faces->at(0).facelets[4].transform = glm::mat4(1.0f);
	_Faces->at(0).facelets[4].init(ColorScheme.data(), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.45f), 90.0f);
}

void
CubeModel::_CreateBlackFaces(const Faceturn& ft)
{
	/*
	** Retrieve the translation vector from the transformation matrix
	** of the center facelet of the turning face
	** Then create a translation matrix equal to 2/3 of the opposite vector 
	*/
	glm::vec3 translation(_Faces->at(ft.face).facelets[4].transform[3]);
	glm::mat4 translateMatrix(glm::translate(glm::mat4(1.0f), -translation / 1.5f));
	glm::vec3 scale(3.0f, 3.0f, 0.0f);

	/* Create two black faces to hide the inside of the cube during the faceturn */
	_InsideFaces[0].transform = glm::scale(translateMatrix * _Faces->at(ft.face).facelets[4].transform, scale);
	_InsideFaces[0].color = nullptr;
	_InsideFaces[1] = _InsideFaces[0];

	_UpdateBlackFaceInstance(1);
}

void
CubeModel::_UpdateBlackFaceInstance(unsigned int idx)
{
	unsigned int offset = Face::NextOffset + sizeof(InstanceData) * idx;

	InstanceData data(_InsideFaces[idx]);
	_FaceletInstances->update(offset, &data, sizeof(InstanceData));
}

void
CubeModel::_TurnFace(Faceturn& ft, double& time)
{
	const Face::FaceTurnDesc& turnInfo = Face::RotationRules[ft.face];
	double newTime = glfwGetTime();
	float stepAngle;

	if (AnimEnabled)
		stepAngle = static_cast<float>((newTime - time) * 90.0 / 0.25 * Delay);
	else
	{
		if (newTime - time < 0.10 / Delay)
			return ;
		stepAngle = ft.finalAngle;
	}
	time = newTime;

	/* Avoid the case where rotating by stepAngle would go further than finalAngle */
	if (ft.finalAngle == 180.0f && ft.currentAngle >= 90.0f)
		stepAngle = std::min(stepAngle, 180.0f - ft.currentAngle);
	else
		stepAngle = static_cast<float>(std::min(fabs(stepAngle), 90.0 - fabs(ft.currentAngle)));

	if (!ft.clockwise)
		stepAngle = -stepAngle;

	/*
	** Apply the rotation to instances affected by the move
	** This is purely visual, it only mutates facelet instances
	*/
	_RotateFaceInstances(ft.face, glm::rotate(glm::mat4(1.0f), glm::radians(stepAngle), turnInfo.RotationAxis));
	ft.currentAngle += stepAngle;

	/*
	** Reassign facelet data between the faces after each quarter-turn
	** Facelet instances are not mutated, their content is just copied to another face
	** The purpose is to sync the internal cube state with what the user sees on screen
	*/
	if (fmod(ft.currentAngle, 90.0f) == 0)
		_RotateFaceData(ft.face, ft.clockwise);
}

void
CubeModel::_RotateFaceInstances(Turn face, const glm::mat4& rotation)
{
	const Face::FaceTurnDesc& turnInfo = Face::RotationRules[face];

	/* Compute the new transformation matrix of each facelet of the turning face */
	for (auto f = _Faces->at(face).facelets.begin(); f != _Faces->at(face).facelets.end(); f++)
		f->transform = rotation * f->transform;
	/* Apply the same rotation to the black square */
	_InsideFaces[0].transform = rotation *_InsideFaces[0].transform;
	
	/* Compute the new transformation matrix of adjacent facelets on other faces */
	for (auto f = turnInfo.AdjacentFaces.begin(); f != turnInfo.AdjacentFaces.end(); f++)
		_RotateRow(_Faces->at(f->face).rows[f->row], rotation);
}

void
CubeModel::_RotateFaceData(Turn face, bool clockwise)
{
	const Face::FaceTurnDesc&	turnInfo(Face::RotationRules[face]);
	std::array<Instance, 9>		copy(_Faces->at(face).facelets);

	/* Reassign facelets of the turning face to simulate a rotation */
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

	/* Swap facelets between rows of adjacent faces */
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
