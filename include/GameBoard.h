/*
 *
 */

#include <GLES2/gl2.h>
#include <string.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

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
	void internalDraw(ShaderProgram* program, const glm::mat4& mvp);
	
	
private:
	glm::mat4 mTransofm;
	CubeGeometry* mCubeGeometry;
	std::vector<Cube*> mCubes;

};

#endif // CUBE_H

