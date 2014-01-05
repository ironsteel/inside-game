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

#include "GameBoard.h"
#include "ShaderProgram.h"
#include "CubeGeometry.h"
#include "Cube.h"
#include "Ray.h"

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
	
// 	glm::mat4 tr = glm::mat4(mTransofm);
// 	
// 	for(int i = 0; i < mNeighbours.size(); ++i) {
// 		
// 		tr = glm::translate(tr, glm::vec3(0, i * 1.8f, 0));
// 		
// 		std::vector<Cube*> v = mNeighbours[i];
// 		for(int j = 0; j < v.size(); j++) {
// 			Cube* cube = v[j];
// 			glm::mat4 mvp = viewProjection * tr * cube->getTransform();
// 			glm::mat4 model = tr * cube->getTransform();
// 			glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(model));
// 			glUniformMatrix3fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));
// 			glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(model));
// 			internalDraw(program, mvp);
// 		}
// 	}
// 	
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
			if((*ci)->hasBase()) 
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
	
	for(int i = 0; i < 6; i++)
	{
		if(i == 0)
			buildNextBoardLevel(mCubes, i * 2, 6 - i, true);
		else 
			buildNextBoardLevel(mNotVisibleCubes, i * 2, 6 - i, false);
	}
	
	/*
	buildNextBoardLevel(mCubes, 0, 6);
	
	buildNextBoardLevel(mNotVisibleCubes, 2, 5);
	
	buildNextBoardLevel(mNotVisibleCubes, 4, 4);
	
	buildNextBoardLevel(mNotVisibleCubes, 6, 3);
	
	buildNextBoardLevel(mNotVisibleCubes, 8, 2);
	
	buildNextBoardLevel(mNotVisibleCubes, 10, 1);*/
}


void GameBoard::buildNextBoardLevel(list<Cube*> &which, float startFrom, int level, bool selected) 
{
	std::vector<Cube* > neighbours;
	float xoffset = 1.4;
	float yoffset = 2;
	float zoffset = 1.4;
	
	float x = 1.4;
	float y = startFrom + 2;
	float z = 1.4;
	int row = 2;
	glm::vec3 pos = glm::vec3(0, startFrom, 0);
	Cube* cube = new Cube(pos);
	which.push_back(cube);
	cube->mSelected = selected;
	if(!mNeighbours.empty() && !selected)
	{
		cube->n = mNeighbours[0];
		
		mNeighbours.erase(mNeighbours.begin());
	}
	neighbours.push_back(cube);
	
	
	for(int i = 1; i < level; ++i) {
		float xnew = x;
		for(int r = 0; r < row; ++r) {
			
			pos = glm::vec3(xnew, y, z);
			cube = new Cube(pos);
			cube->mSelected = selected;
			which.push_back(cube);
			xnew -= 2 * xoffset;
			if(!mNeighbours.empty() && !selected)
			{
				cube->n = mNeighbours[0];
				
				mNeighbours.erase(mNeighbours.begin());
			}
			
			
			if(r > 0 && r < row - 1) {
				neighbours.push_back(cube);
				neighbours.push_back(mNeighbours.back()[1]);				
				mNeighbours.push_back(neighbours);
		
				
				neighbours.clear();
 				neighbours.push_back(cube);

			} else {
				neighbours.push_back(cube);
				if(r == row - 1 && neighbours.size() < 3) {
					neighbours.push_back(mNeighbours[mNeighbours.size() - 2][2]);				
				}
			}
			
		}
		mNeighbours.push_back(neighbours);
		
		neighbours.clear();
		x += xoffset;
		y += yoffset;
		z += zoffset;
		row++;
	}
}