#include "InsideApplication.h"
#include "ShaderUtills.h"
#include "ShaderProgram.h"
#include "GameBoard.h"
#include "TextureUtils.h"
#include <math.h>

InsideApplication::InsideApplication()
{
	cube = new GameBoard();
}

InsideApplication::~InsideApplication()
{
	delete cube;
	delete mShaderProgram;
}

void InsideApplication::init()
{
	GLuint vertexShader = compileShader("../resources/shaders/simple.vsh", GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader("../resources/shaders/simple.fsh", GL_FRAGMENT_SHADER);
	mShaderProgram = new ShaderProgram(vertexShader, fragmentShader);
	if(!mShaderProgram->link()) {
		printf("Cannot link program");
	}
	TextureUtils::loadTexture("../resources/textures/wood.jpg", &mTextureHandle);
	
	cube->initGeometry();
	
	mLookAt = glm::vec3(0, 20, -20);
	mPosition = glm::vec3(0, 0, 0);
	mCameraUp = glm::vec3(0, 1, 0);
	mViewTransform = glm::lookAt(glm::vec3(0, 20, -20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
 	glEnable(GL_DEPTH_TEST);
}

void InsideApplication::drawOneFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(mShaderProgram->getProgramId());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
	
	GLint textureLocation = mShaderProgram->getUniformLocation(u_Sampler);
	glUniform1i(textureLocation, 0);
	
	glm::mat4 mvp = mProjectionTransform * mViewTransform ;
	cube->draw(mShaderProgram,  mvp);
	
}

void InsideApplication::update(double timeSinceLastFrame)
{
	
}

void InsideApplication::reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	mScreenWidth = width;
	mScreenHeight = height;
	mProjectionTransform = glm::perspective(45.0, (double) width / height, 0.1, 100.0);
}

void InsideApplication::onKeyPressed()
{
	
}

void InsideApplication::onPointerDown(int left, int right, double x, double y)
{
	mLeftPressed = (right == 1);
	
	doSelection((float)x, (float)y);
	findIntersection();
	if(mLeftPressed) {
		mLastXPos = mCurrentXPos = x;
		mLastYPos = mCurYPos = y;
	}
}

void InsideApplication::findIntersection()
{
	glm::mat4 viewProjection = mViewTransform;
	cube->intersect(viewProjection, mRayDirection, mRayPos);
}

void InsideApplication::doSelection(float x, float y)
{
	
	float mouseX = x;
	float mouseY = mScreenHeight - y;
	glm::vec3 worldSpaceNear = glm::unProject(glm::vec3(mouseX, mouseY, 0.0),
											  mViewTransform, 
											  mProjectionTransform, 
											  glm::vec4(0, 0, mScreenWidth, mScreenHeight));
	glm::vec3 worldSpaceFar = glm::unProject(glm::vec3(mouseX, mouseY, 1.0),
											 mViewTransform , 
											 mProjectionTransform, 
										  glm::vec4(0, 0, mScreenWidth, mScreenHeight));
	
	
	mRayPos = glm::vec3(worldSpaceNear.x, worldSpaceNear.y, worldSpaceNear.z);
	mRayDirection = glm::vec3(worldSpaceFar.x - worldSpaceNear.x, worldSpaceFar.y - worldSpaceNear.y, worldSpaceFar.z - worldSpaceNear.z);
	mRayDirection = glm::normalize(mRayDirection);
}

void InsideApplication::onPointerMoved(double x, double y)
{
	mCurrentXPos = x;
	mCurYPos = y;
	if(mLeftPressed) {
		if (mCurrentXPos != mLastXPos || mCurYPos != mLastYPos) {
			mViewTransform  = glm::rotate(mViewTransform, glm::degrees(-(float)((mLastXPos - mCurrentXPos)/(mScreenWidth/4))), glm::vec3(0, 1, 0));
			mLastXPos = mCurrentXPos;
			mLastYPos = mCurYPos;
		}
		
	
	}
	
}



