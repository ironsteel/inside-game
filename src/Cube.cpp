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
/*
 *
 */

#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
Cube::Cube(glm::vec3 &pos) :mTransform(glm::mat4(1.0)), mSelected(false), mOutside(false)
{
	mTransform = glm::translate(mTransform, pos);
	mTransform = glm::rotate(mTransform, 45.0f, glm::vec3(0, 1, 0));
}

Cube::~Cube()
{

}

glm::mat4& Cube::getTransform()
{
	return mTransform;
}

bool Cube::hasSupportingNeibours()
{
	for(int i = 0; i < mSupportingCubes.size(); i++) {
		Cube* c = mSupportingCubes[i];
		if(!c->mSelected)
			return false;
	}
	return true;
}

Cube* Cube::getCubeForFreeSpace() 
{
	std::cout << "Suppurted count: " << mSupportedCubes.size() << std::endl;
	for(int i = 0; i < mSupportedCubes.size(); i++) {
		Cube* c = mSupportedCubes[i];
		if(c->hasSupportingNeibours()) 
			return c;
	}
	return NULL;
}
