#include "InsideApplication.h"
#include "ShaderProgram.h"
#include "GameBoard.h"
#include "TextureUtils.h"
#include "ShaderManager.h"
#include <math.h>
#include "ResourceManager.h"
#include "DefaultResourceProvider.h"

InsideApplication::InsideApplication()
{
	mGameBoard = new GameBoard();
	ResourceManager& resManager = ResourceManager::getInstance();
	resManager.setBaseDirectory("../resources/");
	resManager.setResourceProvider(new DefaultResourceProvider());
}

InsideApplication::~InsideApplication()
{
	delete mGameBoard;
	delete mShaderProgram;
}

void InsideApplication::init()
{
	mShaderProgram = ShaderManager::getInstance().createShaderProgram("simple",
																	  "shaders/simple.vsh",
																      "shaders/simple.fsh");
	
	TextureUtils::loadTexture("../resources/textures/wood.png", &mTextureHandle);
	
	mGameBoard->initGeometry();
	
	mLookAt = glm::vec3(0, 20, -20);
	mPosition = glm::vec3(0, 0, 0);
	mCameraUp = glm::vec3(0, 1, 0);
	mViewTransform = glm::lookAt(mLookAt, mPosition, mCameraUp);
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
	
	glm::mat4 mvp = mProjectionTransform * mViewTransform;
	mGameBoard->draw(mShaderProgram,  mvp);
	
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

void InsideApplication::onKeyPressed(int key)
{
	switch (key) {
		case 265:
			mGameBoard->moveLight(0, 1, 0);
			break;
		case 264:
			mGameBoard->moveLight(0, -1, 0);
			break;
		case 263:
			mGameBoard->moveLight(0, 0, 1);
			break;
		case 262:
			mGameBoard->moveLight(0, 0, -1);
			break;
		default:
			return;
	}
}

void InsideApplication::onPointerDown(int left, int right, double x, double y)
{
	mLeftPressed = (right == 1);
	if(mLeftPressed) {
		mLastXPos = mCurrentXPos = x;
		mLastYPos = mCurYPos = y;
		doSelection((float)x, (float)y);
		findIntersection();
	}
}

void InsideApplication::findIntersection()
{
	mGameBoard->intersect(mViewTransform, mRayDirection, mRayPos);
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



