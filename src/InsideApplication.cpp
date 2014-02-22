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

#define GLFW_INCLUDE_NONE

#include <math.h>
#include <GLFW/glfw3.h>
#include <Rocket/Core.h>
#include <Rocket/Debugger/Debugger.h>

#include "InsideApplication.h"
#include "ShaderProgram.h"
#include "GameBoard.h"
#include "TextureUtils.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "DefaultResourceProvider.h"
#include "Camera.h"
#include "Ray.h"
#include "ShellFileInterface.h"
#include "ShellSystemInterface.h"
#include "RenderInterfaceOpenGLES.h"

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
	delete mGameBoard;
	delete mShaderProgram;
	delete mCamera;
	
	delete mRocketGLESRenderer;
	delete mRocketFileInterface;
	delete mRocketSystemInterface;
}

void InsideApplication::init()
{
	mShaderProgram = ShaderManager::getInstance().createShaderProgram("simple", "shaders/simple.vsh", "shaders/simple.fsh");
	
	mGameBoard->initGeometry();
	
	
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

void InsideApplication::terminate()
{
	// Shutdown Rocket.
	mRocketContext->RemoveReference();
	Rocket::Core::Shutdown();
}

void InsideApplication::loadFonts(const char* directory)
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

void InsideApplication::drawOneFrame()
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

void InsideApplication::update(double timeSinceLastFrame)
{
	
}

void InsideApplication::reshape(int width, int height)
{
	if(mRocketGLESRenderer)
		mRocketGLESRenderer->SetViewport(width, height);
	mCamera->windowSizeChanged(width, height);
}

void InsideApplication::onKeyPressed(int key)
{
	if(key == GLFW_KEY_D) {
		mGameBoard->mCurrentCubeColor = DARK;
	} else if(key == GLFW_KEY_L) {
		mGameBoard->mCurrentCubeColor = LIGHT;
	}
}

void InsideApplication::onPointerDown(int button, double x, double y)
{
	mLastXPos = mCurrentXPos = x;
	mLastYPos = mCurYPos = y;
	mLeftPressed = (button == GLFW_MOUSE_BUTTON_LEFT);
	
	mRocketContext->ProcessMouseButtonDown(button, 0);
	
}

void InsideApplication::onPointerUp(int button, double cursorX, double cursorY) 
{
    
	mRocketContext->ProcessMouseButtonUp(button, 0);
	
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
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