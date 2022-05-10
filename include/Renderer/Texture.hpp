/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Texture.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/11 01:24:42 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/11 01:25:53 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_HPP
# define TEXTURE_HPP

# include <string>
# include "Renderer.hpp"

class Texture
{
	public:

		Texture(const std::string& path);
		~Texture();

		void bind(unsigned int slot = 0) const;
		void unbind() const;

		inline int getWidth() const		{ return _width; }
		inline int getHeight() const	{ return _height; }

	private:

		unsigned int	_rendererId;
		std::string		_filepath;
		unsigned char*	_localBuffer;
		int				_width, _height, _bpp;
};

#endif
