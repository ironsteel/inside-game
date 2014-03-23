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



#include <math.h>
#include <GLFW/glfw3.h>

#include "InsideApplication.h"
#include "AppStateManager.h"
#include "ResourceManager.h"
#include "DefaultResourceProvider.h"
#include "GameState.h"


InsideApplication::InsideApplication()
{
	ResourceManager& resManager = ResourceManager::getInstance();
	resManager.setBaseDirectory("../resources/");
	resManager.setResourceProvider(new DefaultResourceProvider());
	mStateManager = new AppStateManager();
	GameState::create(mStateManager, "GameState");
}

InsideApplication::~InsideApplication()
{
	delete mStateManager;
}

void InsideApplication::init()
{
	mStateManager->start(mStateManager->findByName("GameState"));	
}

void InsideApplication::terminate()
{
	mStateManager->shutdown();
}

void InsideApplication::drawOneFrame()
{
	mStateManager->findByName("GameState")->draw(0);
}

void InsideApplication::update(double timeSinceLastFrame)
{
	mStateManager->findByName("GameState")->update(timeSinceLastFrame);
}

void InsideApplication::reshape(int width, int height)
{
	if(mStateManager->findByName("GameState")) {
		mStateManager->findByName("GameState")->reshape(width, height);	
	}
}
void InsideApplication::onKeyPressed(int key)
{
	mStateManager->findByName("GameState")->onKeyPressed(key);
}

void InsideApplication::onPointerDown(int button, double x, double y)
{
	mStateManager->findByName("GameState")->onPointerDown(button, x, y);
}

void InsideApplication::onPointerUp(int button, double cursorX, double cursorY) 
{
	mStateManager->findByName("GameState")->onPointerUp(button, cursorX, cursorY);
}


void InsideApplication::onPointerMoved(double x, double y)
{
	mStateManager->findByName("GameState")->onPointerMoved(x, y);
}
