/*
 *
 */

#include <GLES2/gl2.h>
#include <string.h>

#ifndef CUBE_H
#define CUBE_H

class ShaderProgram;
class Cube
{
public:
    Cube();
    ~Cube();
	void initGeometry();
	void draw(ShaderProgram* program);
	
	
private:
	GLuint mVboIds[3];

};

#endif // CUBE_H
