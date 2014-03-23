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

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "AppState.h"
#include <Rocket/Core.h>


class GameBoard;
class ShaderProgram;
class Camera;

class GameState : public AppState
{
public:
	GameState();
	~GameState();
	DECLARE_APPSTATE_CLASS(GameState)
	
	void enter();
	void exit();
	bool pause();
	void resume();
	
	virtual void onPointerUp(int button, double cursorX, double cursorY);
	virtual void onPointerDown(int button, double x, double y);
	virtual void onPointerMoved(double x, double y);
	virtual void onKeyPressed(int key);	

	virtual void update(double timeSinceLastFrame) {}
	virtual void draw(double timeSinceLastFrame);
	virtual void reshape(int width, int height);
	
private:
	void doSelection(float x, float y);
	
	void loadFonts(const char *directory);
	
private:
	
	ShaderProgram* mShaderProgram;
	
	Camera* mCamera;
	GameBoard* mGameBoard;

	bool mLeftPressed;
	
	float mLastXPos, mCurrentXPos;
	
	float mLastYPos, mCurYPos;
	
	
};

#endif // GAMESTATE_H
