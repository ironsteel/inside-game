/* Inside game
 * 
 * Copyright (C) 2013 Rangel Ivanov
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ShaderUtils.h"
#include "ResourceManager.h"

#include <stdio.h>
#include <stdlib.h>

GLuint ShaderUtils::createProgramAndAttachShaders(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint shaderProgramObject = glCreateProgram();
	
	glAttachShader(shaderProgramObject, vertexShader);
	glAttachShader(shaderProgramObject, fragmentShader);
	
    return shaderProgramObject;
}

bool ShaderUtils::linkProgram(GLuint shaderProgram)
{
	GLint linked;
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
	
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLen);
		char *infoLog = (char*) malloc(sizeof(char) * infoLen);
		glGetProgramInfoLog(shaderProgram, infoLen, NULL, infoLog);
		printf("Error linking shader program\n%s\n", infoLog);
		glDeleteProgram(shaderProgram);
		free(infoLog);
	}
	
	return linked == GL_TRUE;
}


GLuint ShaderUtils::compileShader(const char *src, GLenum shaderType)
{
	GLuint shader;
	GLint compiled;
	
	
	shader = glCreateShader(shaderType);
	
	if(shader == 0)
		return 0;
	glShaderSource(shader, 1, &src, NULL);
	
	glCompileShader(shader);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	
	if (!compiled) {
		GLint infoLen = 0;
		
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		
		if (infoLen > 1) {
			char *infoLog = (char*) malloc(sizeof(char) * infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			printf("Error compiling shader:\n%s\n", infoLog);
			free(infoLog);
		}
		
		glDeleteShader(shader);
		return 0;
	}
	
	return shader;
}

GLuint ShaderUtils::compileShaderFromFile(const char *path, GLenum shaderType) 
{
	
	char *src = ResourceManager::getInstance().getFileBuffer(path);
	
	GLuint shaderHandle = ShaderUtils::compileShader(src, shaderType);
	
	free(src);
	
	return shaderHandle;
}
