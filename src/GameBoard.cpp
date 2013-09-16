/*
 *
 */

#include "GameBoard.h"
#include "ShaderProgram.h"
#include "CubeGeometry.h"
#include "Cube.h"

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
}

void GameBoard::draw(ShaderProgram* program, glm::mat4& viewProjection)
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
		internalDraw(program, mvp, *ci);
	}
	
	mCubeGeometry->unbind(vertIdx, texCoords, normalsIds);
}

void GameBoard::moveLight(float x, float y, float z) 
{
	ligtx += x;
	ligty += y;
	ligtz += z;
}

void GameBoard::internalDraw(ShaderProgram* program, const glm::mat4& mvp, Cube* cube)
{
	GLint mvpId = program->getUniformLocation(u_ModelViewProjection);
	glUniformMatrix4fv(mvpId, 1, GL_FALSE, glm::value_ptr(mvp));
	mCubeGeometry->draw();
}

void GameBoard::update(double time) 
{
	
}

void GameBoard::intersect(glm::mat4& viewProjection, glm::vec3& mRayDirection, glm::vec3& mRayPos) 
{
	
	for (list<Cube*>::iterator ci = mNotVisibleCubes.begin(); ci != mNotVisibleCubes.end(); ++ci)
	{
		glm::mat4 mvp =  mTransofm * (*ci)->getTransform();
		bool selected = (*ci)->mSelected;
		if(!selected && mCubeGeometry->intersect(mvp, mRayDirection, mRayPos)) {
			(*ci)->mSelected = !(*ci)->mSelected;
			mCubes.push_back(*ci);
			mNotVisibleCubes.erase(ci);
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
	buildNextBoardLevel(mCubes, 0, 6);
	buildNextBoardLevel(mNotVisibleCubes, 2, 5);
	buildNextBoardLevel(mNotVisibleCubes, 4, 4);
	buildNextBoardLevel(mNotVisibleCubes, 6, 3);
	buildNextBoardLevel(mNotVisibleCubes, 8, 2);
	buildNextBoardLevel(mNotVisibleCubes, 10, 1);
}


void GameBoard::buildNextBoardLevel(list<Cube*> &which, float startFrom, int level) 
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
	which.push_back(cube);
	
	for(int i = 1; i < level; ++i) {
		float xnew = x;
		for(int r = 0; r < row; ++r) {
			pos = glm::vec3(xnew, y, z);
			cube = new Cube(pos);
			which.push_back(cube);
			xnew -= 2 * xoffset;
		}
		x += xoffset;
		y += yoffset;
		z += zoffset;
		row++;
	}
}