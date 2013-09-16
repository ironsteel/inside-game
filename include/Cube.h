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

#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>

class Cube
{
public:
	Cube(glm::vec3 &pos);
    virtual ~Cube();
	
	glm::mat4& getTransform();
	
	bool mSelected;
	
private:
	glm::mat4 mTransform;
};

#endif // CUBE_H
