/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexArray.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 01:25:21 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:25:27 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "VertexArray.hpp"
#include "Renderer.hpp"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &_rendererId));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &_rendererId));
}

void
VertexArray::bind() const
{
	GLCall(glBindVertexArray(_rendererId));
}

void
VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

void
VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const int firstIndex, bool isInstance)
{
	bind();
	vb.bind();

	const auto& elements = layout.getElements();
	unsigned long offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i + firstIndex));
		GLCall(glVertexAttribPointer(i + firstIndex, element.count, element.type, element.normalized,
			layout.getStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);

		if (isInstance)
		{
			GLCall(glVertexAttribDivisor(i + firstIndex, 1));
		}
	}
}
