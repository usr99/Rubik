/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:48:44 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:04:06 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Renderer.hpp"
#include "Camera.hpp"

glm::mat4 Camera::_projection(glm::perspective(glm::radians(45.0f), RATIO(WINDOW_W, WINDOW_H), 0.1f, 50.0f));
glm::mat4 Camera::_view(1.0f);

void
Camera::updateProjection(int width, int height)
{
	_projection = glm::perspective(glm::radians(45.0f), RATIO(width, height), 0.1f, 50.0f);
}

void
Camera::rotate(const glm::vec3& axis, float angle)
{
	_view = glm::rotate(_view, glm::radians(angle), axis);
}

void
Camera::move(const glm::vec2& direction)
{
	_view = glm::translate(_view, glm::vec3(direction.x, direction.y, 0.0f));
}

void
Camera::zoom(float distance)
{
	_view = glm::translate(_view, glm::vec3(0.0f, 0.0f, distance));
}

glm::mat4
Camera::get()
{
	return _projection * _view;
}
