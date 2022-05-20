/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   VertexBuffer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 01:24:59 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:27:58 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
