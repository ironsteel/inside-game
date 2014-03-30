/*
 * Inside 
 * Copyright (C) 2014  Rangel Ivanov rangelivanov88[at]gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

#include "GameState.h"
#include "ShaderManager.h"
#include "GameBoard.h"
#include "TextureUtils.h"
#include "Camera.h"
#include "Ray.h"
#include "GUI.h"

#include <iostream>

using namespace std;

GameState::GameState()
{
	mGameBoard = new GameBoard();
	mCamera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 20, -20), glm::vec3(0, 1, 0));
	mGameBoard->initGeometry();
	mShaderProgram = ShaderManager::getInstance().createShaderProgram("simple", "shaders/simple.vsh", "shaders/simple.fsh");
}

GameState::~GameState()
{
	if (mGameStateGui != NULL) {
		mGameStateGui->RemoveReference();
	}
	delete mGameBoard;
	delete mShaderProgram;
	delete mCamera;	
}

void GameState::enter()
{
	// Load and show the tutorial document.
	mGameStateGui = GUI::getInstance().getContext()->LoadDocument("../resources/layouts/demo.rml");
	if(mGameStateGui != NULL) {
		mGameStateGui->Show();
	}
}

void GameState::draw(double timeSinceLastFrame)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.4, 0.4, 1);
	
	glEnable(GL_DEPTH_TEST);
 	glUseProgram(mShaderProgram->getProgramId());
 
 	mGameBoard->draw(mShaderProgram,  mCamera->getViewProjection());
}

void GameState::onPointerDown(int button, double x, double y)
{	
	mLastXPos = mCurrentXPos = x;
	mLastYPos = mCurYPos = y;
	mLeftPressed = (button == GLFW_MOUSE_BUTTON_LEFT);
	
}

void GameState::onPointerUp(int button, double cursorX, double cursorY) 
{	
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		doSelection((float)cursorX, (float)cursorY);
		mLeftPressed = false;
	}
}

void GameState::doSelection(float x, float y)
{
	Ray ray = mCamera->getPickingRay(x, y);
	mGameBoard->intersect(mCamera->getViewTransform(), ray);
}

void GameState::reshape(int width, int height) 
{
	mCamera->windowSizeChanged(width, height);	
}

void GameState::onPointerMoved(double x, double y)
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


void GameState::onKeyPressed(int key)
{
	if(key == GLFW_KEY_D) {
		mGameBoard->mCurrentCubeColor = DARK;
	} else if(key == GLFW_KEY_L) {
		mGameBoard->mCurrentCubeColor = LIGHT;
	} else if(key == GLFW_KEY_ESCAPE) {
		popAppState();
	}
}


bool GameState::pause()
{
	mGameStateGui->Hide();
	return true;
}

void GameState::resume()
{
	mGameStateGui->Show();
}

void GameState::exit()
{
	mGameStateGui->Hide();
}
