/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexArray.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 01:24:57 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:28:00 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
