/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:30:45 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/12 07:40:46 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtx/vector_angle.hpp>
# include <glm/gtx/rotate_vector.hpp>

class Camera
{
	public:

		Camera(int width, int height, glm::vec3 position);

		void	Rotate(const glm::vec2& angles);
		void	Move(const glm::vec2& direction);
		void	Zoom(float distance);

		glm::mat4 getMatrix() const;

	private:

		Camera();
		Camera(const Camera& rhs);
		Camera& operator=(const Camera& rhs);

		const glm::mat4	_Projection;
		glm::mat4		_View;

		glm::vec3		_Position;
		glm::vec3		_Orientation;
		glm::vec3		_Up;

		int				_Width;
		int				_Height;
};

#endif
