#ifndef INSIDEAPPLICATION_H
#define INSIDEAPPLICATION_H

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GameBoard;
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
	
	void onPointerDown(int left, int right, double x, double y);
	void onPointerMoved(double x, double y);
	
private:
	
	
	
private:
	ShaderProgram* mShaderProgram;
	GLuint mTextureHandle;
	glm::mat4 mViewTransform;
	glm::mat4 mProjectionTransform;
	GameBoard* cube;
	int mScreenWidth;
	int mScreenHeight;
	bool mLeftPressed;
	
	float mLastXPos ,mCurrentXPos;
	
    float mLastYPos , mCurYPos;
	
};

#endif // INSIDEAPPLICATION_H
