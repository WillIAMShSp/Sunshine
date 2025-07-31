#pragma once
#include <iostream>
#include <math.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "PlanetFace.h"
#include "PlanetSettings.h"
#include "FacePlane.h"


#include "Shader.h"
#include "Texture.h"





class BasePlanet
{
public:

	BasePlanet(const int& resolution, Shader& shader);

	void SetFaces();

	void SetPlanetSettings(PlanetSettings& settings);

	void UpdateFaces();
	
	void UpdateMinMax();

	void SmoothenNormals();

	void Draw();
	
	void DrawDepth(Shader& depthshader);

	int m_resolution = 10;


	


	bool m_resetminmax = true;

private:

	PlanetFace m_planetfaces[6];

	Shader m_shader;
	PlanetSettings m_settings;

	MinMax m_minmax;
	
	VertexBuffer m_vb;

	// Texture m_planetcolor;

	glm::vec3 m_directions[6] =
	{
		glm::vec3(0.f, 1.f, 0.f), // up    0
		glm::vec3(0.f, -1.f, 0.f),// down  1 
		glm::vec3(1.f, 0.f, 0.f), // right 2 
		glm::vec3(-1.f, 0.f, 0.f),// left  3
		glm::vec3(0.f, 0.f, 1.f), // front 4
		glm::vec3(0.f, 0.f, -1.f) // back  5
	};


	FacePlane m_faceplanes[6] =
	{
		
		FacePlane(m_directions[0], m_directions[2]),
		FacePlane(m_directions[1], m_directions[3]),
		FacePlane(m_directions[2], m_directions[5]),
		FacePlane(m_directions[3], m_directions[4]),
		FacePlane(m_directions[4], m_directions[2]),
		FacePlane(m_directions[5], m_directions[3])

	};

	glm::vec3 colors[6] =
	{
		glm::vec3(1.0, 0.0, 0.0),   // Red
		glm::vec3(0.0, 1.0, 0.0),   // Green
		glm::vec3(0.0, 0.0, 1.0),   // Blue
		glm::vec3(1.0, 1.0, 0.0),   // Yellow
		glm::vec3(1.0, 0.0, 1.0),   // Magenta
		glm::vec3(0.0, 1.0, 1.0)    // Cyan


	};

	void ResetMinMax();


	private: //funcs


		void SmoothHorizontal(PlanetFace& frontface, PlanetFace& rightface, PlanetFace& leftface, PlanetFace& backface);
		void SmoothToTheRight(PlanetFace& selectedface, PlanetFace& rightface);


		void SmoothNorthPole(PlanetFace& selectedface, PlanetFace& frontface, PlanetFace& rightface, PlanetFace& leftface, PlanetFace& backface);
		void SmoothSouthPole(PlanetFace& selectedface, PlanetFace& frontface, PlanetFace& rightface, PlanetFace& leftface, PlanetFace& backface);
};


