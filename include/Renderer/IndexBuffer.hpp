#ifndef INDEX_BUFFER_HPP
#define INDEX_BUFFER_HPP

class IndexBuffer
{
	public:
	
		IndexBuffer(const unsigned int *data, unsigned int count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int getCount() const { return _count; }

	private:
	
		unsigned int _rendererId;
		unsigned int _count;
};

#endif
