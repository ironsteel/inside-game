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
#include "InsideApplication.h"
#include "ShaderProgram.h"
#include "GameBoard.h"
#include "TextureUtils.h"
#include "ShaderManager.h"
#include <math.h>
#include "ResourceManager.h"
#include "DefaultResourceProvider.h"
#include "Camera.h"
#include "Ray.h"


InsideApplication::InsideApplication()
{
	mGameBoard = new GameBoard();
	ResourceManager& resManager = ResourceManager::getInstance();
	resManager.setBaseDirectory("../resources/");
	resManager.setResourceProvider(new DefaultResourceProvider());
	mCamera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 20, -20), glm::vec3(0, 1, 0));
}

InsideApplication::~InsideApplication()
{
	glDeleteTextures(1, &mTextureHandle);
	delete mGameBoard;
	delete mShaderProgram;
	delete mCamera;
}

void InsideApplication::init()
{
	mShaderProgram = ShaderManager::getInstance().createShaderProgram("simple",
																	  "shaders/simple.vsh",
																      "shaders/simple.fsh");
	
	TextureUtils::loadTexture("../resources/textures/wood.png", &mTextureHandle);
	
	mGameBoard->initGeometry();
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
	
	mGameBoard->draw(mShaderProgram,  mCamera->getViewProjection());
	
}

void InsideApplication::update(double timeSinceLastFrame)
{
	
}

void InsideApplication::reshape(int width, int height)
{
	mCamera->windowSizeChanged(width, height);
}

void InsideApplication::onKeyPressed(int key)
{
	
}

void InsideApplication::onPointerDown(MouseButton button, double x, double y)
{
	mLastXPos = mCurrentXPos = x;
	mLastYPos = mCurYPos = y;
	mLeftPressed = (button == LEFT);
}

void InsideApplication::onPointerUp(MouseButton button, double cursorX, double cursorY) 
{
    
	if(button == LEFT) {
		doSelection((float)cursorX, (float)cursorY);
		mLeftPressed = false;
	}
}
void InsideApplication::doSelection(float x, float y)
{
	Ray ray = mCamera->getPickingRay(x, y);
	mGameBoard->intersect(mCamera->getViewTransform(), ray);
}

void InsideApplication::onPointerMoved(double x, double y)
{
	mCurrentXPos = x;
	mCurYPos = y;
	if(mLeftPressed) {
 		if (mCurrentXPos != mLastXPos || mCurYPos != mLastYPos) {
			mCamera->rotate(-(float)((mLastXPos - mCurrentXPos)/(mCamera->mScreenWidth/4)), glm::vec3(0, 1, 0));
			mLastXPos = mCurrentXPos;
			mLastYPos = mCurYPos;
		}
	}
}