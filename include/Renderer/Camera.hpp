/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:30:45 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:03:58 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>

class Camera
{
	public:

		static void	updateProjection(int width, int height);
		static void	rotate(const glm::vec3& axis, float angle);
		static void	move(const glm::vec2& direction);
		static void	zoom(float distance);

		static glm::mat4 get();

	private:

		Camera();
		Camera(const Camera& rhs);
		Camera& operator=(const Camera& rhs);

		static glm::mat4	_projection;
		static glm::mat4	_view;
};

#endif
