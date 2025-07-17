#pragma once

#include <vector>

struct PlanetSettings
{

	struct LayerSettings
	{

		
		LayerSettings() = default;

		void SetSettings(const float& roughnessvalue = 1.f, const float& strengthvalue = 1.f, const float& persistancevalue = 1.f, const int& amountofoctaves = 1.f)
		{
			roughness = roughnessvalue;
			strength = strengthvalue;
			persistance = persistancevalue;
			octaves = amountofoctaves;

		}


		float roughness = 1.f;
		float strength = 1.f;
		float persistance = 1.f;
		int octaves = 1;


		

	};



	PlanetSettings(const float& radiusvalue = 1.f, const float& massvalue = 1.f, const float& minimumvalue = 0.f,const unsigned int& numoflayersvalue = 0, glm::vec3 centreofnoise = glm::vec3(0.f), bool shouldusefirstlayerasmask = false)
	{
		radius = radiusvalue;
		mass = massvalue;
		minvalue = minimumvalue;
		numoflayers = numoflayersvalue;
		noisecentre = centreofnoise;
		usefirstlayerasmask = shouldusefirstlayerasmask;
		perlinlayers.resize(10);

	}

	



	std::vector<LayerSettings> perlinlayers;

	float radius = 1;
	float mass = 1;

	float minvalue = 0;
	int numoflayers = 0;
	glm::vec3 noisecentre = glm::vec3(0.f);

	bool usefirstlayerasmask = false;
	


};