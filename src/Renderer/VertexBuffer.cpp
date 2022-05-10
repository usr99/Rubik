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
