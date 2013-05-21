/*
 *
 */

#ifndef CUBEGEOMETRY_H
#define CUBEGEOMETRY_H

#include <GLES2/gl2.h>
#include <string.h>
#include <stdlib.h>

class CubeGeometry
{
public:
    CubeGeometry();
    virtual ~CubeGeometry();
	void initGeometry();
	void bindBuffers(GLint vertexId, GLint textureCoordsId);
	void unbind(GLint vertexId, GLint textureCoordsId);
	void draw();
	
private:
	GLuint mVboIds[3];
	GLfloat* mCubeVertices;
	
};

#endif // CUBEGEOMETRY_H
