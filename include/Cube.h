/*
 *
 */

#include <GLES2/gl2.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifndef CUBE_H
#define CUBE_H

class ShaderProgram;
class Cube
{
public:
    Cube();
    ~Cube();
	void initGeometry();
	void draw(ShaderProgram* program, glm::mat4& viewProjection);
	void update(double time);
	void internalDraw(ShaderProgram* program, const glm::mat4& viewProjection);
	
	
private:
	GLuint mVboIds[3];
	glm::mat4 mModelTransform;
	glm::mat4 mParentTransofm;

};

#endif // CUBE_H
