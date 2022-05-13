/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubeModel.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:23:24 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/13 01:58:53 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE_MODEL_HPP
# define CUBE_MODEL_HPP

# define GLM_ENABLE_EXPERIMENTAL

# include <ctime>
# include <memory>
# include <array>
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtx/rotate_vector.hpp>

# include "FaceletCube.hpp"
# include "VertexArray.hpp"
# include "IndexBuffer.hpp"
# include "Shader.hpp"
# include "Texture.hpp"

class CubeModel
{
	public:

		CubeModel(Shader& shader, const FaceletCube& rhs = FaceletCube());

		void Render();
		void PushMove(int index, float angle);

		bool	AnimEnabled;
		float	AnimSpeed;

	private:

		enum Turn { U, R, F, D, L, B };

		struct Vertex;
		struct Instance;
		struct Faceturn;
		struct Face;

		void _UpdateInstances();

		void _TurnFace(Faceturn& ft);
		void _RotateFaceInstances(Turn face, const glm::mat4& rotation);
		void _RotateFaceData(Turn face, bool clockwise);

		void _RotateRow(std::array<Instance*, 3>& row, const glm::mat4& rotation);
		void _SwapFaceRows(const std::array<Instance*, 3>& from, const std::array<Instance*, 3> to, bool reverse);

		VertexArray						_VAO;
		Texture							_FaceletTex;

		std::unique_ptr<VertexBuffer>	_FaceletVertices;
		std::unique_ptr<VertexBuffer>	_FaceletInstances;
		std::unique_ptr<IndexBuffer>	_FaceletIndices;
		
		std::list<Faceturn>				_WaitingMoves;

		bool							_Updated;

		std::array<Face, 6>*			_Faces;

		static std::array<glm::vec3, 6>	ColorScheme;
};

struct CubeModel::Vertex
{
	glm::vec3 pos;
	glm::vec2 tex;
};

struct CubeModel::Instance
{
	Instance();

	void init(const glm::vec3& color, const glm::vec3& rot, glm::vec3 translation, float angle);

	glm::vec3 color;
	glm::mat4 transform;
};

struct CubeModel::Face
{
	enum Row {
		Left = 0, Right	 = 2,
		Top	 = 1, Bottom = 3
	};

	struct FaceRowDesc
	{
		Turn face;
		Row	 row;
		bool reverse;
	};

	struct FaceTurnDesc
	{
		std::array<FaceRowDesc, 4>	AdjacentFaces;
		glm::vec3					RotationAxis;
	};

	Face(const Facelet* source, const glm::vec3& rotation, float angle);

	void RotateFaceletsData();
	void RotateRow(Row index, const glm::mat4& RotationMatrix);

	std::array<Instance, 9>					facelets;
	std::array<std::array<Instance*, 3>, 4>	rows;
	unsigned int							offset;

	static unsigned int NextOffset;
	static const std::array<FaceTurnDesc, 6> RotationRules;
};

struct CubeModel::Faceturn
{
	Faceturn(int index, float angle);

	Turn	face;
	float	finalAngle;
	float	currentAngle;
	bool	clockwise;
};

#endif
