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
private:
	glm::mat4 mTransform;
};

#endif // CUBE_H
