/*
 *
 */

#include "Cube.h"
#include "ShaderProgram.h"

Cube::Cube()
{

}

Cube::~Cube()
{
	glDeleteBuffers(3, mVboIds);
}

void Cube::draw(ShaderProgram* program)
{
	GLint vertIdx = program->getAttributeLocation(a_Position);
	glEnableVertexAttribArray(vertIdx);
	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[0]);
	glVertexAttribPointer(
		vertIdx, // attribute
		3,                 // number of elements per vertex, here (x,y,z)
	GL_FLOAT,          // the type of each element
	GL_FALSE,          // take our values as-is
	0,                 // no extra data between each position
	0                  // offset of first element
	);
	
	GLint texCoords = program->getAttributeLocation(a_TexCoords);
	glEnableVertexAttribArray(texCoords);
	glBindBuffer(GL_ARRAY_BUFFER, mVboIds[1]);
	glVertexAttribPointer(
		texCoords, // attribute
		2,                  // number of elements per vertex, here (x,y)
	GL_FLOAT,           // the type of each element
	GL_FALSE,           // take our values as-is
	0,                  // no extra data between each position
	0                   // offset of first element
	);
	
	/* Push each element in buffer_vertices to the vertex shader */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
	
	
	glDrawElements(GL_TRIANGLES, 72/sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
	
	glDisableVertexAttribArray(vertIdx);
	glDisableVertexAttribArray(texCoords);
}

void Cube::initGeometry()
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
	glGenBuffers(3, mVboIds);
	glBindBuffer(GL_ARRAY_BUFFER,  mVboIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVboIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
}
