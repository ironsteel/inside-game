/*
 * Inside Game
 * Copyright (C) 2013  Rangel Ivanov <rangelivanov88[at]gmail.com>
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

#include <glm/glm.hpp>

#ifndef CAMERA_H
#define CAMERA_H

class Ray;

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 upVector);
    ~Camera();
	
	void windowSizeChanged(int width, int height);
	
	void setTarget(glm::vec3 target);
	void setPosition(glm::vec3 position);
	void setUpVector(glm::vec3 up);
	
	glm::mat4 getViewProjection();
	glm::mat4& getViewTransform();
	
	Ray getPickingRay(float mousePosX, float mousePosY);
	
	void rotate(float angleRad, glm::vec3 axis); 
	
	void invalidate();	
	
	int mScreenWidth, mScreenHeight;
private:
	
	double mFov, mNear, mFar; 
	
	glm::mat4 mViewTransform;
	glm::mat4 mProjectionTransform;
	glm::mat4 mViewProjectionTransform;
	
	glm::detail::tvec3< glm::mediump_float > mLookAt;
	glm::detail::tvec3< glm::mediump_float > mPosition;
	glm::detail::tvec3< glm::mediump_float > mCameraUp;
};

#endif // CAMERA_H
