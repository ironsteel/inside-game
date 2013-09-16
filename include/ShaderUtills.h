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
#ifndef SHADERUTILLS_H
#define SHADERUTILLS_H

#include <GLES2/gl2.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "ResourceManager.h"

bool loadShaderFromFile(const char *path, std::string& out) {
    std::ifstream file;
    file.open(path);

    if (!file) {
        return false;
    }

    std::stringstream stream;

    stream << file.rdbuf();

    file.close();

    out = stream.str();

	return true;
}

GLuint compileShader(const char *path, GLenum shaderType)
{
    GLuint shader;
    GLint compiled;
    
	
	char *src = ResourceManager::getInstance().getFileBuffer(path);

    shader = glCreateShader(shaderType);

    if(shader == 0)
        return 0;
	glShaderSource(shader, 1, const_cast<const GLchar* const*>(&src), NULL);
	free(src);
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

#endif // SHADERUTILLS_H
