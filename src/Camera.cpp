/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2013  <copyright holder> <email>
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

#define GLM_FORCE_RADIANS

#include "Camera.h"
#include <Ray.h>
#include <glm/ext.hpp>
#include <GLES2/gl2.h>

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 upVector) :
mFov(45), mNear(0.1), mFar(100), 
mProjectionTransform(glm::mat4()), mViewTransform(glm::mat4()),
mLookAt(target), mPosition(position), mCameraUp(upVector)
{
	invalidate();
}

void Camera::windowSizeChanged(int width, int height)
{
	glViewport(0, 0, width, height);
	mScreenWidth = width;
	mScreenHeight = height;
	mProjectionTransform = glm::perspective(glm::radians(mFov), (double) mScreenWidth / mScreenHeight, mNear, mFar);
}

void Camera::setPosition(glm::vec3 position)
{
	mPosition = position;
	invalidate();
}

void Camera::setTarget(glm::vec3 target)
{
	mLookAt = target;
	invalidate();
}


void Camera::setUpVector(glm::vec3 upVector)
{
	mCameraUp = upVector;
	invalidate();
}

void Camera::rotate(float angleRad, glm::vec3 axis)
{
	mViewTransform = glm::rotate(mViewTransform, angleRad, axis);
}


void Camera::invalidate()
{
	mViewTransform = glm::lookAt(mLookAt, mPosition, mCameraUp);
}


glm::mat4 Camera::getViewProjection() 
{
	return mProjectionTransform * mViewTransform;
}

glm::mat4& Camera::getViewTransform()
{
	return mViewTransform;
}


Ray Camera::getPickingRay(float mousePosX, float mousePosY)
{

	float mouseX = mousePosX;
	float mouseY = mScreenHeight - mousePosY;
	glm::vec3 worldSpaceNear = glm::unProject(glm::vec3(mouseX, mouseY, 0.0),
											  mViewTransform, 
										   mProjectionTransform, 
										   glm::vec4(0, 0, mScreenWidth, mScreenHeight));
	glm::vec3 worldSpaceFar = glm::unProject(glm::vec3(mouseX, mouseY, 1.0),
											 mViewTransform , 
										  mProjectionTransform, 
										  glm::vec4(0, 0, mScreenWidth, mScreenHeight));
	
	
	glm::vec3 rayPos = glm::vec3(worldSpaceNear.x, worldSpaceNear.y, worldSpaceNear.z);
	glm::vec3 direction = glm::vec3(worldSpaceFar.x - worldSpaceNear.x, worldSpaceFar.y - worldSpaceNear.y, worldSpaceFar.z - worldSpaceNear.z);
	direction = glm::normalize(direction);
	return Ray(rayPos, direction);
}


Camera::~Camera()
{

}
