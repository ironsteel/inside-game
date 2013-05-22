/*
 *
 */

#include "GameBoard.h"
#include "ShaderProgram.h"
#include "CubeGeometry.h"
#include "Cube.h"

GameBoard::GameBoard() : 
	mCubeGeometry(new CubeGeometry()), 
	mCubes(0), 
	mTransofm(glm::mat4(1.0))
{
	mTransofm = glm::mat4(1.0);
	mTransofm = glm::translate(mTransofm, glm::vec3(0, 0, -8.2));
	mTransofm = glm::rotate(mTransofm, 55.0f, glm::vec3(1, 0, 0));
}

GameBoard::~GameBoard()
{
	size_t size = mCubes.size();
	for(size_t i = 0; i < size; i++) {
		delete mCubes[i];
	}
}

void GameBoard::draw(ShaderProgram* program, glm::mat4& viewProjection)
{
	GLint vertIdx = program->getAttributeLocation(a_Position);
	GLint texCoords = program->getAttributeLocation(a_TexCoords);
	
	mCubeGeometry->bindBuffers(vertIdx, texCoords);
	
	for(size_t i = 0; i < mCubes.size(); i++) {
		glm::mat4 mvp = viewProjection * mTransofm * mCubes[i]->getTransform();
		internalDraw(program, mvp, mCubes[i]);
	}
	
	mCubeGeometry->unbind(vertIdx, texCoords);
 	
}


void GameBoard::internalDraw(ShaderProgram* program, const glm::mat4& mvp, Cube* cube)
{
	GLint mvpId = program->getUniformLocation(u_ModelViewProjection);
	glUniformMatrix4fv(mvpId, 1, GL_FALSE, glm::value_ptr(mvp));
	
	
	GLint selected = program->getUniformLocation(u_Selected);
	if(cube->mSelected) {
		glUniform1i(selected, 1);
	} else {
		glUniform1i(selected, 0);
	}
	
	mCubeGeometry->draw();
}

void GameBoard::update(double time) 
{
	mTransofm = glm::translate(mTransofm, glm::vec3(0, 6, 0));
	float angle =  time / 1000.0 * 75.0;
	mTransofm = glm::rotate(mTransofm,angle, glm::vec3(0, 0, 1));
	mTransofm = glm::translate(mTransofm, glm::vec3(0, -6, 0));
}

void GameBoard::intersect(glm::mat4 viewProjection, glm::vec3 mRayDirection, glm::vec3 mRayPos) 
{
	for(int i = mCubes.size() - 1; i >=0; i--) {
		glm::mat4 mvp =  mTransofm * mCubes[i]->getTransform();
		if(mCubeGeometry->intersect(mvp, mRayDirection, mRayPos)) {
			mCubes[i]->mSelected = !mCubes[i]->mSelected;
			break;
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
	float xoffset = 1.4;
	float yoffset = 2;
	float zoffset = 1.4;
	
	float x = 1.4;
	float y = 2;
	float z = 1.4;
	int row = 2;
	glm::vec3 pos = glm::vec3(0);
	Cube* cube = new Cube(pos);
	mCubes.push_back(cube);
	
	for(int i = 1; i < 6; ++i) {
		float xnew = x;
		for(int r = 0; r < row; ++r) {
			pos = glm::vec3(xnew, y, z);
			cube = new Cube(pos);
			mCubes.push_back(cube);
			xnew -= 2 * xoffset;
		}
		x += xoffset;
		y += yoffset;
		z += zoffset;
		row++;
	}

	buildNextBoardLevel(2, 5);
	buildNextBoardLevel(4, 4);
	buildNextBoardLevel(6, 3);
	buildNextBoardLevel(8, 2);
	buildNextBoardLevel(10, 1);
}


void GameBoard::buildNextBoardLevel(float startFrom, int level) 
{
	float xoffset = 1.4;
	float yoffset = 2;
	float zoffset = 1.4;
	
	float x = 1.4;
	float y = startFrom + 2;
	float z = 1.4;
	int row = 2;
	glm::vec3 pos = glm::vec3(0, startFrom, 0);
	Cube* cube = new Cube(pos);
	mCubes.push_back(cube);
	
	for(int i = 1; i < level; ++i) {
		float xnew = x;
		for(int r = 0; r < row; ++r) {
			pos = glm::vec3(xnew, y, z);
			cube = new Cube(pos);
			mCubes.push_back(cube);
			xnew -= 2 * xoffset;
		}
		x += xoffset;
		y += yoffset;
		z += zoffset;
		row++;
	}
}