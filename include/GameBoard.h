/*
 *
 */

#include <GLES2/gl2.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <list>
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
	void draw(ShaderProgram* program, glm::mat4& viewProjection);
	void update(double time);
	void internalDraw(ShaderProgram* program, const glm::mat4& mvp, Cube* cube);
    void intersect(glm::mat4& viewProjection, glm::vec3& mRayDirection, glm::vec3& mRayPos);
	glm::mat4& getTransform();
	void moveLight(float x, float y, float z);
	
private:
	void buildNextBoardLevel(std::list<Cube*>& which, float startFrom, int level);
	
	
private:
	glm::mat4 mTransofm;
	CubeGeometry* mCubeGeometry;
	std::list<Cube*> mCubes;
	std::list<Cube*> mNotVisibleCubes;
	float ligtx = 5;
	float ligty = 70;
	float ligtz = 20;

};

#endif // CUBE_H

