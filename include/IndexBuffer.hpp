/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IndexBuffer.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 01:24:47 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:24:49 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INDEX_BUFFER_HPP
# define INDEX_BUFFER_HPP

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
