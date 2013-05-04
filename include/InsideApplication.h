#ifndef INSIDEAPPLICATION_H
#define INSIDEAPPLICATION_H

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Cube;
class ShaderProgram;


class InsideApplication
{
public:
	InsideApplication();
	~InsideApplication();
	
public:
	void init();
	
	void drawOneFrame();
	void update(double timeSinceLastFrame);
	
	void reshape(int width, int height);
	void onKeyPressed();
	
	void onPointerDown(int x, int y);
	void onPointerMoved(double x, double y);
	
private:
	
	
	
	
private:
	ShaderProgram* mShaderProgram;
	GLuint texture;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	Cube* cube;
	
};

#endif // INSIDEAPPLICATION_H
