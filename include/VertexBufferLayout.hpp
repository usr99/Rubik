/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexBufferLayout.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 01:25:01 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/21 00:55:11 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERTEX_BUFFER_LAYOUT_HPP
# define VERTEX_BUFFER_LAYOUT_HPP

# include <vector>
# include <assert.h>

template<typename T>
struct always_false: std::false_type {};

struct VertexBufferElement
{
	unsigned int	type;
	unsigned int	count;
	unsigned char	normalized;

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
