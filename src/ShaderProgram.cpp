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
#include "ShaderProgram.h"


ShaderProgram::ShaderProgram(GLuint vertexShader, GLuint fragmentShader)
:mVertexShader(vertexShader), mFragmentShader(fragmentShader), mShaderProgramObject(0)
{
}

ShaderProgram::~ShaderProgram()
{
	glDeleteShader(mVertexShader);
	glDeleteShader(mFragmentShader);
	glDetachShader(mShaderProgramObject, mVertexShader);
	glDetachShader(mShaderProgramObject, mFragmentShader);
	glDeleteProgram(mShaderProgramObject);
}


bool ShaderProgram::link()
{
	bool linkStatus = true;
	mShaderProgramObject = glCreateProgram();
	GLint linked;

	glAttachShader(mShaderProgramObject, mVertexShader);
	glAttachShader(mShaderProgramObject, mFragmentShader);

	glBindAttribLocation(mShaderProgramObject, 0, a_Position.c_str());
	glBindAttribLocation(mShaderProgramObject, 1, a_TexCoords.c_str());
	glBindAttribLocation(mShaderProgramObject, 2, a_Normal.c_str());

	glLinkProgram(mShaderProgramObject);

	glGetProgramiv(mShaderProgramObject, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(mShaderProgramObject, GL_INFO_LOG_LENGTH, &infoLen);
		char *infoLog = (char*) malloc(sizeof(char) * infoLen);
		glGetProgramInfoLog(mShaderProgramObject, infoLen, NULL, infoLog);
		printf("Error linking shader program\n%s\n", infoLog);
		glDeleteProgram(mShaderProgramObject);
		free(infoLog);
		linkStatus = false;
	}




	if(!initUniformLocations())
		linkStatus = false;


	if(!initAttributeLocations())
		linkStatus = false;
			
	if(!linkStatus)
		throw std::exception();

	return true;
}

GLint ShaderProgram::getAttributeLocation(const string& attributeName)
{
    return attributes[attributeName];
}

GLint ShaderProgram::getUniformLocation(const string& uniformName)
{
    return uniforms[uniformName];
}

bool ShaderProgram::initUniformLocations()
{
	
	initUniform(u_ModelViewProjection);
	initUniform(u_lightPosition);
	initUniform(u_NormalMatrix);
	initUniform(u_Sampler);
	initUniform(u_modelMatrix);
	initUniform(u_selected);
	
	return true;
	
}
bool ShaderProgram::initUniform(std::string name)
{
	GLint uniformLocation = glGetUniformLocation(mShaderProgramObject, name.c_str());
	if(uniformLocation == -1) {
		printf("Cannot find %s", name.c_str());
		return false;
	}
	uniforms[name] = uniformLocation;
	return true;
}

bool ShaderProgram::initAttributeLocations()
{
	
	initAttribute(a_Position);
	initAttribute(a_TexCoords);
	initAttribute(a_Normal);
	
    return true;
}

bool ShaderProgram::initAttribute(std::string name)
{
	GLint pos = glGetAttribLocation(mShaderProgramObject, name.c_str());
	if(pos == -1) {
		printf("Cannot find %s", name.c_str());
		return false;
	}
	attributes[name] =  pos;
	return true;
}


GLuint ShaderProgram::getProgramId()
{
    return mShaderProgramObject;
}
