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

#include <GLES2/gl2.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class GameBoard;
class ShaderProgram;


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
	
	void onPointerDown(int left, int right, double x, double y);
	void onPointerMoved(double x, double y);
	
private:
	void doSelection(float x, float y);
    void findIntersection();

	
	
private:
	ShaderProgram* mShaderProgram;
	GLuint mTextureHandle;
	glm::mat4 mViewTransform;
	glm::mat4 mProjectionTransform;
	GameBoard* mGameBoard;
	int mScreenWidth;
	int mScreenHeight;
	bool mLeftPressed;
	
	float mLastXPos ,mCurrentXPos;
	
    float mLastYPos , mCurYPos;
    glm::detail::tvec3< glm::mediump_float > mLookAt;
    glm::detail::tvec3< glm::mediump_float > mPosition;
    glm::detail::tvec3< glm::mediump_float > mCameraUp;
	
	glm::vec3 mRayPos;
	glm::vec3 mRayDirection;
	
};

#endif // INSIDEAPPLICATION_H
