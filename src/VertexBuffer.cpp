/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexBuffer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 01:25:23 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:25:27 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VertexBuffer.hpp"
#include "Renderer.hpp"

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
	GLCall(glGenBuffers(1, &_rendererId));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &_rendererId));
}

void VertexBuffer::update(unsigned int offset, const void *data, unsigned int size)
{
	bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void
VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _rendererId));
}

void
VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
