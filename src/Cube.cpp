/*
 *
 */

#include "Cube.h"
#include <glm/gtc/matrix_transform.hpp>

Cube::Cube(glm::vec3 &pos) :mTransform(glm::mat4(1.0)), mSelected(false)
{
	mTransform = glm::translate(mTransform, pos);
	mTransform = glm::rotate(mTransform, 45.0f, glm::vec3(0, 1, 0));
}

Cube::~Cube()
{

}

glm::mat4& Cube::getTransform()
{
	return mTransform;
}
