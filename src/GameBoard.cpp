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

#define GLM_FORCE_RADIANS

#include "GameBoard.h"
#include "ShaderProgram.h"
#include "CubeGeometry.h"
#include "Cube.h"
#include "Ray.h"
#include "TextureUtils.h"
#include <iostream>

GameBoard::GameBoard() : 
	mCubeGeometry(new CubeGeometry()), 
	mDarkCubes(0), mNotVisibleCubes(0),
	mTransform(glm::mat4(1.0)),
	mCurrentCubeColor(DARK)
{
	mTransform = glm::translate(mTransform, glm::vec3(0, 0, -8.2));
	mTransform = glm::rotate(mTransform, glm::radians(55.0f), glm::vec3(1, 0, 0));
 	mPyramidFaces.push_back(vector<Cube*>());
 	mPyramidFaces.push_back(vector<Cube*>());
 	mPyramidFaces.push_back(vector<Cube*>()); 	
}

GameBoard::~GameBoard()
{
	while(!mDarkCubes.empty()) {
		delete mDarkCubes.front();
		mDarkCubes.pop_front();
	}
	
	while(!mLightCubes.empty()) {
		delete mLightCubes.front();
		mLightCubes.pop_front();
	}
	
	while(!mPyramidBase.empty()) {
		delete mPyramidBase.front();
		mPyramidBase.pop_front();
	}
	
	while(!mNotVisibleCubes.empty()) {
		delete mNotVisibleCubes.front();
		mNotVisibleCubes.pop_front();
	}
	
	delete mCubeGeometry;
}

void GameBoard::draw(ShaderProgram* program, glm::mat4 viewProjection)
{
	
	GLint textureLocation = program->getUniformLocation(u_Sampler);
	
	
	
	GLint ligtPositionId = program->getUniformLocation(u_lightPosition);
	glUniform3f(ligtPositionId, ligtx, ligty, ligtz);
	
	GLint vertIdx = program->getAttributeLocation(a_Position);
	GLint texCoords = program->getAttributeLocation(a_TexCoords);
	GLint normalsIds =  program->getAttributeLocation(a_Normal);

	
	mCubeGeometry->bindBuffers(vertIdx, texCoords, normalsIds);
	

	glUniform1i(textureLocation, 0);
	drawCubeList(mPyramidBase, program, viewProjection);	
	
	drawCubeList(mDarkCubes, program, viewProjection);
	
	glUniform1i(textureLocation, 1);
	drawCubeList(mLightCubes, program, viewProjection);
	
	mCubeGeometry->unbind(vertIdx, texCoords, normalsIds);
}

void GameBoard::drawCubeList(std::list<Cube*>& which, ShaderProgram* program, glm::mat4 viewProjection) 
{
	
	GLint normalMatrixId = program->getUniformLocation(u_NormalMatrix);
	GLint modelMatrixId = program->getUniformLocation(u_modelMatrix);
	GLint selectedId = program->getUniformLocation(u_selected);
	
	for (list<Cube*>::iterator ci = which.begin(); ci != which.end(); ++ci) {
		glm::mat4 mvp = viewProjection * mTransform * (*ci)->getTransform();
		glm::mat4 model = mTransform * (*ci)->getTransform();
		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(model));
		glUniformMatrix3fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(modelMatrixId, 1, GL_FALSE, glm::value_ptr(model));
		internalDraw(program, mvp);
	}
	
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
		glm::mat4 mvp =  mTransform * (*ci)->getTransform();
		bool selected = (*ci)->mSelected;
		if(!selected && mCubeGeometry->intersect(mvp, ray)) {
			if((*ci)->hasSupportingNeibours()) {
				(*ci)->mSelected = !(*ci)->mSelected;
				if(mCurrentCubeColor == LIGHT)
					mLightCubes.push_back(*ci);
				else 
					mDarkCubes.push_back(*ci);
				
				
				mNotVisibleCubes.erase(ci);
				break;
			}
		}
	}
}

glm::mat4& GameBoard::getTransform()
{
	return mTransform;
}

void GameBoard::initGeometry()
{
	TextureUtils::loadTexture("../resources/textures/wood.png", &mDarkWoodTexture);
	TextureUtils::loadTexture("../resources/textures/light-wood.png", &mLightWoodTexture);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mDarkWoodTexture);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mLightWoodTexture);
	
	
	mCubeGeometry->initGeometry();
	int level = 6;
	for(int i = 0; i < level; i++) {
		if(i == 0)
			buildNextBoardLevel(mPyramidBase, i * 2, level - i, true);
		else 
			buildNextBoardLevel(mNotVisibleCubes, i * 2, level - i, false);
	}
}

Cube* GameBoard::addToSupportedCube(Cube* supportedCube, int index) 
{
	Cube* supportingCube = mLevels.back()[index];
	supportingCube->mSupportedCubes.push_back(supportedCube);
	return supportingCube;
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
			
			// These are the cubes that are not placed on the pyramid
			if(!selected) {
				vector<Cube*> neighbours;
				neighbours.push_back(addToSupportedCube(cube, indexInLevel));
				neighbours.push_back(addToSupportedCube(cube, indexInLevel + row));
				neighbours.push_back(addToSupportedCube(cube, indexInLevel + row + 1));
				cube->mSupportingCubes = neighbours;	
				
				
				// Logic for separating the cube which
				// are exterior
				
				// This is the top level cube (on the apex), which has 
				// its 3 faces visible
				if(level == 1) {
					mPyramidFaces[0].push_back(cube);
					mPyramidFaces[1].push_back(cube);
					mPyramidFaces[2].push_back(cube);
				} else if(i == 0) {
					// This is the the first cube of every layer of 
					// the pyramid which can be seen from the east and west sides
					if(r == 0) {
						mPyramidFaces[0].push_back(cube);
						mPyramidFaces[1].push_back(cube);
					}
					
				} else if(i == level-1) {
					// These are the cubes that lay on the 
					// north side of the pyramid
					
					// This cube is also lays on the east side
					if(r == 0) {
						mPyramidFaces[1].push_back(cube);
						mPyramidFaces[2].push_back(cube);
					} else if(r == row - 1) {
						// This cube is also lays on the west side
						mPyramidFaces[2].push_back(cube);
						mPyramidFaces[0].push_back(cube);
					} else {
						mPyramidFaces[2].push_back(cube);
					}
				}  else {
					if(r == 0) {
						// If its the first cube in the row it lays
						// on the east side
						mPyramidFaces[1].push_back(cube);
					} else if(r == row - 1) {
						// Else it lays on the west side
						mPyramidFaces[0].push_back(cube);
					}
				} 
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