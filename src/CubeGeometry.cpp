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

#include "CubeGeometry.h"
#include <glm/ext.hpp>


CubeGeometry::CubeGeometry() : mCubeVertices(0)
{

}

void CubeGeometry::bindBuffers(GLint vertexId, GLint textureCoordsId, GLint normalsId)
{
	glEnableVertexAttribArray(vertexId);
	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
	glVertexAttribPointer(
		vertexId, // attribute
		3,                 // number of elements per vertex, here (x,y,z)
	GL_FLOAT,          // the type of each element
	GL_FALSE,          // take our values as-is
	0,                 // no extra data between each position
	0                  // offset of first element
	);
	
	glEnableVertexAttribArray(textureCoordsId);
	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
	glVertexAttribPointer(
		textureCoordsId, // attribute
		2,                  // number of elements per vertex, here (x,y)
	GL_FLOAT,           // the type of each element
	GL_FALSE,           // take our values as-is
	0,                  // no extra data between each position
	0                   // offset of first element
	);
	
	glEnableVertexAttribArray(normalsId);
	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[4]);
	glVertexAttribPointer(
		normalsId, // attribute
		3,                 // number of elements per vertex, here (x,y,z)
	GL_FLOAT,          // the type of each element
	GL_FALSE,          // take our values as-is
	0,                 // no extra data between each position
	0                  // offset of first element
	);
	
	
	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
}

void CubeGeometry::unbind(GLint vertexId, GLint textureCoordsId, GLint normalsId)
{
	glDisableVertexAttribArray(vertexId);
	glDisableVertexAttribArray(textureCoordsId);
	glDisableVertexAttribArray(normalsId);
}

void CubeGeometry::draw()
{
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

bool CubeGeometry::intersect(glm::mat4 &mvp, glm::vec3 &mRayDirection, glm::vec3 &mRayPos) 
{
	
	glm::vec3 bary;
	for(int i = 0; i < 36; i+=3) {
		
		glm::vec3 verts[3];
		
		for(int j = 0; j < 3; j++) {
			glm::vec4 trangleVerts = glm::vec4(
				(float)mCubeVertices[mCubeElements[i + j] * 3],
				(float)mCubeVertices[mCubeElements[i + j] * 3 + 1],
				(float)mCubeVertices[mCubeElements[i + j] * 3 + 2], 
											   1);
			trangleVerts = (mvp * trangleVerts);
			verts[j] = trangleVerts.xyz();
			
		}
		if(glm::intersectRayTriangle(mRayPos, mRayDirection, verts[0], verts[1], verts[2], bary)) {
			return true;
		}
	}
	return false;
}


void CubeGeometry::initGeometry()
{
	GLfloat cube_vertices[] = {
		// front
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// top
		-1.0,  1.0,  1.0,
		1.0,  1.0,  1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
		// back
		1.0, -1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0,  1.0, -1.0,
		1.0,  1.0, -1.0,
		// bottom
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0,  1.0,
		-1.0, -1.0,  1.0,
		// left
		-1.0, -1.0, -1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		// right
		1.0, -1.0,  1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		1.0,  1.0,  1.0,
	};
	glGenBuffers(4, mVboIds);
	glBindBuffer(GL_ARRAY_BUFFER,  mVboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	
	mCubeVertices = (GLfloat *) malloc(sizeof(cube_vertices));
	memcpy(mCubeVertices, cube_vertices, sizeof(cube_vertices));
	
	GLfloat cube_texcoords[2*4*6] = {
		// front
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};
	for (int i = 1; i < 6; i++)
		memcpy(&cube_texcoords[i*4*2], &cube_texcoords[0], 2*4*sizeof(GLfloat));
	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texcoords), cube_texcoords, GL_STATIC_DRAW);
	
	GLushort cube_elements[] = {
		// front
		0,  1,  2,
		2,  3,  0,
		// top
		4,  5,  6,
		6,  7,  4,
		// back
		8,  9, 10,
		10, 11,  8,
		// bottom
		12, 13, 14,
		14, 15, 12,
		// left
		16, 17, 18,
		18, 19, 16,
		// right
		20, 21, 22,
		22, 23, 20,
	};
	
	mCubeElements = (GLshort *) malloc(sizeof(cube_elements));
	memcpy(mCubeElements, cube_elements, sizeof(cube_elements));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
	
	GLfloat cube_normals[] = {
		// front
		0.0, 0.0,  1.0,
		0.0, 0.0,  1.0,
		0.0, 0.0,  1.0,
		0.0, 0.0,  1.0,
		// top
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		0.0,  1.0,  0.0,
		// back
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		0.0, 0.0, -1.0,
		// bottom
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		// left
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		-1.0, 0.0, 0.0,
		// right
		1.0, 0.0,  0.0,
		1.0, 0.0,  0.0,
		1.0, 0.0,  0.0,
		1.0, 0.0,  0.0,
	};
	glBindBuffer(GL_ARRAY_BUFFER,  mVboIds[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
	
}

CubeGeometry::~CubeGeometry()
{
   free(mCubeVertices);
   free(mCubeElements);
   glDeleteBuffers(4, mVboIds);
}
