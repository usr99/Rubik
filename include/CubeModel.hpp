/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CubeModel.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:23:24 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/14 02:00:20 by mamartin         ###   ########.fr       */
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

		void		Render();
		void		PushMove(int index, float angle);
		void		ApplySequence(const std::list<std::string>& seq);
		CubieCube	toCubieCube() const;

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

			glm::vec3 *color;
			glm::mat4 transform;
		};

		struct Faceturn
		{
			Faceturn(int index, float angle);

			Turn face;
			float finalAngle;
			float currentAngle;
			bool clockwise;
		};

		void _TurnFace(Faceturn& ft);
		void _RotateFaceInstances(Turn face, const glm::mat4& rotation);
		void _RotateFaceData(Turn face, bool clockwise);

		void _RotateRow(std::array<Instance*, 3>& row, const glm::mat4& rotation);
		void _SwapFaceRows(const std::array<Instance*, 3>& from, const std::array<Instance*, 3> to, bool reverse);

		template <typename T, typename U, unsigned int count, unsigned int stickers_count>
		std::array<T, count> _ConvertCubies(const Facelet reference[count][stickers_count]) const;

		VertexArray						_VAO;
		Texture							_FaceletTex;

		std::unique_ptr<VertexBuffer>	_FaceletVertices;
		std::unique_ptr<VertexBuffer>	_FaceletInstances;
		std::unique_ptr<IndexBuffer>	_FaceletIndices;
		
		std::list<Faceturn>				_WaitingMoves;
		std::array<Face, 6>*			_Faces;
};

#endif
