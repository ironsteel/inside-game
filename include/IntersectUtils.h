/*
 *
 */

#ifndef INTERSECTUTILS_H
#define INTERSECTUTILS_H

#include <glm/glm.hpp>

class IntersectUtils
{

public:
	static bool intersectWithTriangle(glm::vec3 rayPos, glm::vec3 rayDir, glm::vec3* triangleVers);
	
};

#endif // INTERSECTUTILS_H
