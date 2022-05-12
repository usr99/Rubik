/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:48:44 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/12 08:26:37 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Renderer.hpp"
#include "Camera.hpp"

Camera::Camera(int width, int height, glm::vec3 position)
	: _Projection(glm::perspective(glm::radians(45.0f), RATIO(WINDOW_W, WINDOW_H), 0.1f, 50.0f)),
		_Position(position), _Orientation(0.0f, 0.0f, -1.0f), _Up(0.0f, 1.0f, 0.0f),
		_Width(width), _Height(height)
{
	_View = glm::lookAt(_Position, _Position + _Orientation, _Up);
}

void
Camera::Rotate(const glm::vec2& angles)
{
	_Position = glm::rotate(_Position, glm::radians(angles.x), glm::vec3(-1.0f, 0.0f, 0.0f));
	_Orientation = glm::rotate(_Orientation, glm::radians(angles.x), glm::vec3(-1.0f, 0.0f, 0.0f));
	_Up = glm::rotate(_Up, glm::radians(angles.x), glm::vec3(-1.0f, 0.0f, 0.0f));

	_Position = glm::rotate(_Position, glm::radians(angles.y), glm::vec3(0.0f, -1.0f, 0.0f));
	_Orientation = glm::rotate(_Orientation, glm::radians(angles.y), glm::vec3(0.0f, -1.0f, 0.0f));

	_View = glm::lookAt(_Position, _Position + _Orientation, _Up);
}

void
Camera::Move(const glm::vec2& direction)
{
	_View = glm::translate(_View, glm::vec3(direction.x, direction.y, 0.0f));
}

void
Camera::Zoom(float distance)
{
	_View = glm::translate(_View, glm::vec3(0.0f, 0.0f, distance));
}

glm::mat4
Camera::getMatrix() const
{
	return _Projection * _View;
}
