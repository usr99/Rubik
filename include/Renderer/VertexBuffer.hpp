#ifndef VERTEX_BUFFER_HPP
# define VERTEX_BUFFER_HPP

class VertexBuffer
{
	public:
	
		VertexBuffer(const void *data, unsigned int size);
		~VertexBuffer();

		void update(unsigned int offset, const void *data, unsigned int size);

		void bind() const;
		void unbind() const;

	private:
	
		unsigned int _rendererId;
};

#endif
