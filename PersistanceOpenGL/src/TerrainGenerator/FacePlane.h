#pragma once
#include "glm/glm.hpp"


struct FacePlane
{

	FacePlane() = default;
	FacePlane(const glm::vec3& updirection, const glm::vec3& xaxis)
		:updirection(updirection), xaxis(xaxis)
	{


	};

	glm::vec3 updirection;
	glm::vec3 xaxis;

};
