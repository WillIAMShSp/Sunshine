#pragma once
#include <iostream>
#include "Array2D.h"
#include "PerlinImplementation.h"
#include "glm/glm.hpp"




class PerlinNoise
{
public:


	PerlinNoise() = default;

	void ApplyPerlinNoiseToArray2D(Array2D& lattice, const float& frequency, const float& amplitude, const int& octaves, const float& persistence = 0.5f);
	
	glm::vec3 Apply3DPerlinNoiseToVec3(glm::vec3& vec3, const float& frequency, const float& amplitude, const int& octaves, const float& persistence = 0.5f);

	float PerlinNoise3D(glm::vec3 & vec3, const float& frequency, const float& amplitude, const int& octaves, const float& persistence = 0.5f, const glm::vec3& centre = glm::vec3(0));


private:


	siv::PerlinNoise::seed_type seed = 123456u;

	siv::PerlinNoise perlin{ seed };




};

