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
#ifndef INSIDEAPPLICATION_H
#define INSIDEAPPLICATION_H

#define GLM_FORCE_RADIANS



class GameBoard;
class ShaderProgram;
class Camera;

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
	void onKeyPressed(int key);
	
	void onPointerDown(int button, double x, double y);
	void onPointerUp(int button, double cursorX, double cursorY);
	
	void onPointerMoved(double x, double y);
	
	
private:
	void doSelection(float x, float y);
	
private:
	ShaderProgram* mShaderProgram;

	Camera* mCamera;
	GameBoard* mGameBoard;
	
	bool mLeftPressed;
	
	float mLastXPos, mCurrentXPos;
	
    float mLastYPos, mCurYPos;
	
};

#endif // INSIDEAPPLICATION_H
