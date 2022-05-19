/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubeModel.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:23:24 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/19 02:22:30 by mamartin         ###   ########.fr       */
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
# include "CubieCube.hpp"

class CubeModel
{
	public:

		CubeModel(Shader& shader, const FaceletCube& rhs = FaceletCube());
		~CubeModel();

		void		Render();
		void		PushMove(int index, float angle);
		void		ApplySequence(const std::list<std::string>& seq);
		FaceletCube	toFaceletCube() const;
		CubieCube	toCubieCube() const;
		void		setState(const FaceletCube& from);

		bool	AnimEnabled;
		float	Delay;

		static std::array<glm::vec3, 6>	ColorScheme;

	private:

		struct Face;

		enum Turn { U, R, F, D, L, B };

		struct Vertex
		{
			glm::vec3 pos;
			glm::vec2 tex;
		};

		struct Instance
		{
			Instance();

			void init(glm::vec3 *color, const glm::vec3 &rot, glm::vec3 translation, float angle);

			glm::vec3 		*color;
			glm::mat4 		transform;
		};

		struct InstanceData
		{
			InstanceData();
			InstanceData(const Instance& rhs);

			void init(const Instance& rhs);

			glm::vec3 		color;
			glm::mat4 		transform;
		};

		struct Faceturn
		{
			Faceturn(int index, float angle);

			Turn face;
			float finalAngle;
			float currentAngle;
			bool clockwise;
		};

		void _CreateBlackFaces(const Faceturn& ft);
		void _UpdateBlackFaceInstance(unsigned int idx);
		void _TurnFace(Faceturn& ft, double& time);

		void _RotateFaceInstances(Turn face, const glm::mat4& rotation);
		void _RotateFaceData(Turn face, bool clockwise);

		void _RotateRow(std::array<Instance*, 3>& row, const glm::mat4& rotation);
		void _SwapFaceRows(const std::array<Instance*, 3>& from, const std::array<Instance*, 3> to, bool reverse);

		VertexArray						_VAO;
		Texture							_FaceletTex;
		Texture							_42IconTex;

		std::unique_ptr<VertexBuffer>	_FaceletVertices;
		std::unique_ptr<VertexBuffer>	_FaceletInstances;
		std::unique_ptr<IndexBuffer>	_FaceletIndices;
		
		std::list<Faceturn>				_WaitingMoves;
		std::array<Face, 6>*			_Faces;
		std::array<Instance, 2>			_InsideFaces;
};

#endif
