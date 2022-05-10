#ifndef VERTEX_ARRAY_HPP
# define VERTEX_ARRAY_HPP

# include "VertexBuffer.hpp"
# include "VertexBufferLayout.hpp"

class VertexArray
{
	public:

		VertexArray();
		~VertexArray();

		void bind() const;
		void unbind() const;

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, const int firstIndex, bool isInstance);

	private:

		unsigned int _rendererId;
};

#endif
