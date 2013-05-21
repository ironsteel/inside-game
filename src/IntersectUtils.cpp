/*
 *
 */

#include "IntersectUtils.h"
#include <glm/core/type.hpp>
#include <glm/gtx/quaternion.hpp>



bool IntersectUtils::intersectWithTriangle(glm::vec3 rayPos, glm::vec3 rayDir, glm::vec3* triangleVers) 
{
	glm::vec3 edge1 =  triangleVers[1] - triangleVers[0];
	glm::vec3 edge2 =  triangleVers[2] - triangleVers[0];
	
	// Compute the determinant.
	glm::vec3 directionCrossEdge2 =  glm::cross(rayDir, edge2);
	
	
	float determinant = glm::dot(directionCrossEdge2, edge1);
	// If the ray and triangle are parallel, there is no collision.
	if (determinant > -.0000001f && determinant < .0000001f) {
		return false;
	}
	
	float inverseDeterminant = 1.0f / determinant;
	
	// Calculate the U parameter of the intersection point.
	glm::vec3 distanceVector = rayPos -  triangleVers[0];
	
	float triangleU = glm::dot(directionCrossEdge2, distanceVector);
	triangleU *= inverseDeterminant;
	
	// Make sure the U is inside the triangle.
	if (triangleU < 0 || triangleU > 1) {
		return false;
	}
	
	// Calculate the V parameter of the intersection point.
	glm::vec3 distanceCrossEdge1 = glm::cross(distanceVector, edge1);
	
	
	float triangleV = glm::dot(rayDir, distanceCrossEdge1);
	triangleV *= inverseDeterminant;
	
	// Make sure the V is inside the triangle.
	if (triangleV < 0 || triangleU + triangleV > 1) {
		return false;
	}
	
	// Get the distance to the face from our ray origin
	float rayDistance = glm::dot(distanceCrossEdge1, edge2);
	rayDistance *= inverseDeterminant;
	
	
	// Is the triangle behind us?
	if (rayDistance < 0) {
		rayDistance *= -1;
		return false;
	}
	return true;
}