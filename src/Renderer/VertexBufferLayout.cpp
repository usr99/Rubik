# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>

#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"

unsigned int
VertexBufferElement::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
	}
	ASSERT(false);
	return 0;
}

VertexBufferLayout::VertexBufferLayout() :
	_stride(0) {}

template <>
void
VertexBufferLayout::push<float>(unsigned int count)
{
	_elements.push_back({ GL_FLOAT, count, GL_FALSE });
	_stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template <>
void
VertexBufferLayout::push<unsigned int>(unsigned int count)
{
	_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
	_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template <>
void
VertexBufferLayout::push<unsigned char>(unsigned int count)
{
	_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
	_stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}

template <>
void
VertexBufferLayout::push<glm::mat4>(unsigned int count)
{
	while (count--)
	{
		push<float>(4);
		push<float>(4);
		push<float>(4);
		push<float>(4);
	}
}
