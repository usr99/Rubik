/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 23:37:56 by mamartin          #+#    #+#             */
/*   Updated: 2022/05/10 23:47:33 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Shader.hpp"
#include "Renderer.hpp"

Shader::Shader(const std::string& filepath) :
	_filepath(filepath)
{
	const std::string vertexSource = _parseShader(filepath + ".vs");
	const std::string fragmentSource = _parseShader(filepath + ".fs");
	_id = _createShader(vertexSource, fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(_id));
}

void
Shader::bind() const
{
	GLCall(glUseProgram(_id));
}

void
Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

void
Shader::setUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(_getUniformLocation(name), value));
}

void Shader::setUniform3f(const std::string& name, float v0, float v1, float v2)
{
	GLCall(glUniform3f(_getUniformLocation(name), v0, v1, v2));
}

void
Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(_getUniformLocation(name), v0, v1, v2, v3));
}

void
Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(_getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

std::string
Shader::_parseShader(const std::string &filepath)
{
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss;

	while (getline(stream, line))
		ss << line << '\n';
	stream.close();
	return ss.str();
}

unsigned int
Shader::_createShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = _compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = _compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

unsigned int
Shader::_compileShader(unsigned int type, const std::string &source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char *src = source.c_str();

	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *message = reinterpret_cast<char *>(alloca(length * sizeof(char)));
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cerr << "Failed to compile shader "
				  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
				  << " shader !\n";
		std::cerr << message << "\n";

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

int
Shader::_getUniformLocation(const std::string& name)
{
	if (_uniformLocationCache.find(name) != _uniformLocationCache.end())
		return _uniformLocationCache[name];

	GLCall(int location = glGetUniformLocation(_id, name.c_str()));
	if (location == -1)
		std::cout << "Warning: uniform '" << name << "' doesn't exist !" << std::endl;
	else
		_uniformLocationCache[name] = location;
	return location;
}
