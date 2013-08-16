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
