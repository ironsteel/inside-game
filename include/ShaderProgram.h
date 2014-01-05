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
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <GLES2/gl2.h>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "ShaderConstants.h"


using namespace std;

class ShaderProgram 
{

public:
    ShaderProgram(GLuint vertexShader, GLuint fragmentShader);
    virtual ~ShaderProgram();
		
	virtual bool link();
	GLint getAttributeLocation(const string& attributeName);
	GLint getUniformLocation(const string& uniformName);
	virtual GLuint getProgramId();
    
protected:
    virtual bool initUniformLocations();
    virtual bool initAttributeLocations();

private:
	bool initUniform(std::string name);
	bool initAttribute(std::string name);

private:
    GLuint mVertexShader;
    GLuint mFragmentShader;    
    
    GLuint mShaderProgramObject;
    map<string, GLuint> attributes;
    map<string, GLint> uniforms;
    
};

#endif // SHADERPROGRAM_H
