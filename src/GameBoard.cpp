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

#include "GameBoard.h"
#include "ShaderProgram.h"
#include "CubeGeometry.h"
#include "Cube.h"
#include "Ray.h"
#include <iostream>

GameBoard::GameBoard() : 
	mCubeGeometry(new CubeGeometry()), 
	mCubes(0), mNotVisibleCubes(0),
	mTransofm(glm::mat4(1.0))
{
	mTransofm = glm::translate(mTransofm, glm::vec3(0, 0, -8.2));
	mTransofm = glm::rotate(mTransofm, 55.0f, glm::vec3(1, 0, 0));
}

GameBoard::~GameBoard()
{
	while(!mCubes.empty()) {
		delete mCubes.front();
		mCubes.pop_front();
	}
	
	while(!mNotVisibleCubes.empty()) {
		delete mNotVisibleCubes.front();
		mNotVisibleCubes.pop_front();
	}
	
	delete mCubeGeometry;
}

void GameBoard::draw(ShaderProgram* program, glm::mat4 viewProjection)
{
	GLint ligtPositionId = program->getUniformLocation(u_lightPosition);
	glUniform3f(ligtPositionId, ligtx, ligty, ligtz);
	
	GLint vertIdx = program->getAttributeLocation(a_Position);
	GLint texCoords = program->getAttributeLocation(a_TexCoords);
	GLint normalsIds =  program->getAttributeLocation(a_Normal);
	
	GLint normalMatrixId = program->getUniformLocation(u_NormalMatrix);
	GLint modelMatrixId = program->getUniformLocation(u_modelMatrix);
	mCubeGeometry->bindBuffers(vertIdx, texCoords, normalsIds);
		
	for (list<Cube*>::iterator ci = mCubes.begin(); ci != mCubes.end(); ++ci) {
		glm::mat4 mvp = viewProjection * mTransofm * (*ci)->getTransform();
		glm::mat4 model = mTransofm * (*ci)->getTransform();
		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(model));
		glUniformMatrix3fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(model));
		internalDraw(program, mvp);
	}
	
	mCubeGeometry->unbind(vertIdx, texCoords, normalsIds);
}

void GameBoard::moveLight(float x, float y, float z) 
{
	ligtx += x;
	ligty += y;
	ligtz += z;
}

void GameBoard::internalDraw(ShaderProgram* program, const glm::mat4& mvp)
{
	GLint mvpId = program->getUniformLocation(u_ModelViewProjection);
	glUniformMatrix4fv(mvpId, 1, GL_FALSE, glm::value_ptr(mvp));
	mCubeGeometry->draw();
}

void GameBoard::update(double time) 
{
	
}

void GameBoard::intersect(glm::mat4& viewProjection, Ray& ray) 
{
	
	for (list<Cube*>::iterator ci = mNotVisibleCubes.begin(); ci != mNotVisibleCubes.end(); ++ci)
	{
		glm::mat4 mvp =  mTransofm * (*ci)->getTransform();
		bool selected = (*ci)->mSelected;
		if(!selected && mCubeGeometry->intersect(mvp, ray)) {
			if((*ci)->hasSupportingNeibours()) 
			{
				(*ci)->mSelected = !(*ci)->mSelected;
				mCubes.push_back(*ci);
				mNotVisibleCubes.erase(ci);
				break;
			}
		}
	}
}

glm::mat4& GameBoard::getTransform()
{
	return mTransofm;
}

void GameBoard::initGeometry()
{
	mCubeGeometry->initGeometry();
	int level = 6;
	for(int i = 0; i < level; i++) {
		if(i == 0)
			buildNextBoardLevel(mCubes, i * 2, level - i, true);
		else 
			buildNextBoardLevel(mNotVisibleCubes, i * 2, level - i, false);
	}
	
}


void GameBoard::buildNextBoardLevel(list<Cube*> &which, float startFrom, int level, bool selected) 
{
	std::vector<Cube* > currentLevelCubes;
	float xoffset = 1.4;
	float yoffset = 2;
	float zoffset = 1.4;
	
	float x = 0;
	float y = startFrom;
	float z = 0;
	int row = 1;
	glm::vec3 pos = glm::vec3(0, startFrom, 0);
	Cube* cube = NULL;
	int indexInLevel = 0;
	for(int i = 0; i < level; ++i) {
		float xnew = x;
		for(int r = 0; r < row; ++r) {
			pos = glm::vec3(xnew, y, z);
			cube = new Cube(pos);
			cube->mSelected = selected;
			
			if(!selected) {
				vector<Cube*> neighbours;
				neighbours.push_back(mLevels.back()[indexInLevel]);
				neighbours.push_back(mLevels.back()[indexInLevel + row]);
				neighbours.push_back(mLevels.back()[indexInLevel + row + 1]);
				cube->mNeighbours = neighbours;	
			}
			which.push_back(cube);
			currentLevelCubes.push_back(cube);
			
			xnew -= 2 * xoffset;
			indexInLevel++;
		}
		
		x += xoffset;
		y += yoffset;
		z += zoffset;
		row++;
	}
	mLevels.push_back(currentLevelCubes);
}