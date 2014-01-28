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

#include <GLES2/gl2.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <list>

class Ray;
class CubeGeometry;

#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class ShaderProgram;
class Cube;
class GameBoard
{
public:
	GameBoard();
	~GameBoard();
	void initGeometry();
	void draw(ShaderProgram* program, glm::mat4 viewProjection);
	void update(double time);
	void internalDraw(ShaderProgram* program, const glm::mat4& mvp);
	void intersect(glm::mat4& viewProjection, Ray& ray);
	glm::mat4& getTransform();
	void moveLight(float x, float y, float z);
	
private:
	void buildNextBoardLevel(std::list<Cube*>& which, float startFrom, int level, bool selected);
		
private:
	glm::mat4 mTransofm;
	CubeGeometry* mCubeGeometry;
	std::list<Cube*> mCubes;
	std::list<Cube*> mNotVisibleCubes;
	std::vector<Cube*> mAllCubes;
	std::vector< std::vector<Cube*> > mPyramidFaces;

	
	std::vector< std::vector<Cube*> > mLevels;
	float ligtx = 5;
	float ligty = 70;
	float ligtz = 20;

};

#endif // CUBE_H

