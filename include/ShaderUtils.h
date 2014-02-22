/* Inside game

   Copyright (C) 2013 Rangel Ivanov

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <GLES2/gl2.h>


class ShaderUtils
{
public:
	static GLuint compileShader(const char *src, GLenum shaderType);
	static GLuint compileShaderFromFile(const char *path, GLenum shaderType);
	
	static GLuint createProgramAndAttachShaders(GLuint vertexShader, GLuint fragmentShader);
	static bool linkProgram(GLuint shaderProgram);
};



#endif // SHADERUTILS_H
