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
#include <Rocket/Core.h>
#include <Rocket/Debugger/Debugger.h>
#include "ShaderManager.h"
#include "GameBoard.h"
#include "TextureUtils.h"
#include "Camera.h"
#include "Ray.h"
#include "ShellFileInterface.h"
#include "ShellSystemInterface.h"
#include "RenderInterfaceOpenGLES.h"


GameState::GameState()
{
	mGameBoard = new GameBoard();
	mCamera = new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 20, -20), glm::vec3(0, 1, 0));
}

GameState::~GameState()
{       
	delete mGameBoard;
	delete mShaderProgram;
	delete mCamera;
	
	delete mRocketGLESRenderer;
	delete mRocketFileInterface;
	delete mRocketSystemInterface;
	
}

void GameState::enter()
{

	
	mGameBoard->initGeometry();
	mShaderProgram = ShaderManager::getInstance().createShaderProgram("simple", "shaders/simple.vsh", "shaders/simple.fsh");
	
	
	mRocketSystemInterface = new ShellSystemInterface();
	Rocket::Core::SetSystemInterface(mRocketSystemInterface);
	
	// Rocket initialisation.
	mRocketFileInterface = new ShellFileInterface("../resources/");
	Rocket::Core::SetFileInterface(mRocketFileInterface);
	
	
	mRocketGLESRenderer = new RenderInterfaceOpenGLES();
	mRocketGLESRenderer->SetViewport(mCamera->mScreenWidth, mCamera->mScreenHeight);
	Rocket::Core::SetRenderInterface(mRocketGLESRenderer);
	
	
	Rocket::Core::Initialise();
	
	mRocketContext = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(mCamera->mScreenWidth, mCamera->mScreenHeight));
	if (mRocketContext == NULL)
	{
		Rocket::Core::Shutdown();
		
	}
	
	Rocket::Debugger::Initialise(mRocketContext);
	
	loadFonts("../resources/fonts/");
	
	// Load and show the tutorial document.
	Rocket::Core::ElementDocument* document = mRocketContext->LoadDocument("../resources/layouts/demo.rml");
	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();
	}
	
	
}


void GameState::loadFonts(const char* directory)
{
	Rocket::Core::String font_names[4];
	font_names[0] = "Delicious-Roman.otf";
	font_names[1] = "Delicious-Italic.otf";
	font_names[2] = "Delicious-Bold.otf";
	font_names[3] = "Delicious-BoldItalic.otf";
	
	for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
	{
		Rocket::Core::FontDatabase::LoadFontFace(Rocket::Core::String(directory) + font_names[i]);
	}
}

void GameState::draw(double timeSinceLastFrame)
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.4, 0.4, 0.4, 1);
	
	
	glEnable(GL_DEPTH_TEST);
 	glUseProgram(mShaderProgram->getProgramId());
 
 	mGameBoard->draw(mShaderProgram,  mCamera->getViewProjection());
	

	glDisable(GL_DEPTH_TEST);
	
	mRocketContext->Update();
	mRocketContext->Render();
}

void GameState::onPointerDown(int button, double x, double y)
{
	if (mRocketContext->GetHoverElement() != mRocketContext->GetRootElement()) {
		mRocketContext->ProcessMouseButtonDown(button, 0);
		return;
	}
	
	mLastXPos = mCurrentXPos = x;
	mLastYPos = mCurYPos = y;
	mLeftPressed = (button == GLFW_MOUSE_BUTTON_LEFT);
	
}

void GameState::onPointerUp(int button, double cursorX, double cursorY) 
{
	if (mRocketContext->GetHoverElement() != mRocketContext->GetRootElement()) {
		mRocketContext->ProcessMouseButtonUp(button, 0);
		return;
	}
	
	
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
	if(mRocketGLESRenderer) {
		mRocketGLESRenderer->SetViewport(width, height);
		mRocketContext->SetDimensions(Rocket::Core::Vector2i(width, height));
	}
	mCamera->windowSizeChanged(width, height);
	
}

void GameState::onPointerMoved(double x, double y)
{
	mRocketContext->ProcessMouseMove((int) x,(int) y, 0);
	
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
	}
}


bool GameState::pause()
{
	return true;
}

void GameState::resume()
{

}

void GameState::exit()
{
	// Shutdown Rocket.
	mRocketContext->RemoveReference();
	Rocket::Core::Shutdown();
}
