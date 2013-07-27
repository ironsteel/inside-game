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

class CubeGeometry
{
public:
    CubeGeometry();
    virtual ~CubeGeometry();
	void initGeometry();
	void bindBuffers(GLint vertexId, GLint textureCoordsId, GLint normalsId);
	void unbind(GLint vertexId, GLint textureCoordsId, GLint normalsId);
	void draw();
    bool intersect(glm::mat4 &mvp, glm::vec3 &mRayDirection, glm::vec3 &mRayPos);
	
private:
	GLuint mVboIds[4];
	GLfloat* mCubeVertices;
	GLshort* mCubeElements;
	
};

#endif // CUBEGEOMETRY_H
