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
/*
 *
 */

#ifndef CUBEGEOMETRY_H
#define CUBEGEOMETRY_H

#define GLM_SWIZZLE

#include <GLES2/gl2.h>
#include <string.h>
#include <stdlib.h>
#include <glm/glm.hpp>

class Ray;

class CubeGeometry
{
public:
    CubeGeometry();
    virtual ~CubeGeometry();
	void initGeometry();
	void bindBuffers(GLint vertexId, GLint textureCoordsId, GLint normalsId);
	void unbind(GLint vertexId, GLint textureCoordsId, GLint normalsId);
	void draw();
	bool intersect(glm::mat4& mvp, Ray& ray);
	
private:
	GLuint mVboIds[4];
	GLfloat* mCubeVertices;
	GLshort* mCubeElements;
	
};

#endif // CUBEGEOMETRY_H
