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
#include "MenuState.h"
#include "GUI.h"


InsideApplication::InsideApplication()
{
	ResourceManager& resManager = ResourceManager::getInstance();
	resManager.setBaseDirectory("../resources/");
	resManager.setResourceProvider(new DefaultResourceProvider());
	mStateManager = new AppStateManager();

}

InsideApplication::~InsideApplication()
{

}

void InsideApplication::init()
{
	GUI::getInstance().init("../resources/");
	GameState::create(mStateManager, "GameState");
	MenuState::create(mStateManager, "MenuState");
	mStateManager->start(mStateManager->findByName("MenuState"));
}

void InsideApplication::terminate()
{
	delete mStateManager;
}

void InsideApplication::drawOneFrame()
{
	mStateManager->getActiveState()->draw(0);
	GUI::getInstance().updateGUI();
}

void InsideApplication::update(double timeSinceLastFrame)
{
	mStateManager->getActiveState()->update(timeSinceLastFrame);
}

void InsideApplication::reshape(int width, int height)
{
	if(AppState *activeState = mStateManager->getActiveState()) {
		activeState->reshape(width, height);
	}
	GUI::getInstance().setViewPortSize(width, height);
}
void InsideApplication::onKeyPressed(int key)
{
	mStateManager->getActiveState()->onKeyPressed(key);
}

void InsideApplication::onPointerDown(int button, double x, double y)
{
	if(GUI::getInstance().processMouseDown(button, 0)) {
		return;
	}
	mStateManager->getActiveState()->onPointerDown(button, x, y);
}

void InsideApplication::onPointerUp(int button, double cursorX, double cursorY) 
{
	if(GUI::getInstance().processMouseUp(button, 0)) {
		return;
	}
	mStateManager->getActiveState()->onPointerUp(button, cursorX, cursorY);
}

void InsideApplication::onPointerMoved(double x, double y)
{
	GUI::getInstance().processMouseMove(x, y);
	mStateManager->getActiveState()->onPointerMoved(x, y);
}

bool InsideApplication::isRunning()
{
	return !mStateManager->m_bShutdown;
}
