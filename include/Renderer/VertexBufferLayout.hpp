#ifndef VERTEX_BUFFER_LAYOUT_HPP
# define VERTEX_BUFFER_LAYOUT_HPP

# include <GL/glew.h>
# include <vector>
# include <assert.h>

template<typename T>
struct always_false: std::false_type {};

struct VertexBufferElement
{
	unsigned int	type;
	unsigned int	count;
	GLboolean		normalized;

	static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
	public:

		VertexBufferLayout();

		template <typename T>
		void push(unsigned int count);

		inline const std::vector<VertexBufferElement>& getElements() const	{ return _elements; }
		inline unsigned int getStride() const								{ return _stride; }

	private:

		std::vector<VertexBufferElement>	_elements;
		unsigned int						_stride;
};

template <typename T>
void
VertexBufferLayout::push(unsigned int count)
{
	(void)count;
	assert(false);
}

#endif
