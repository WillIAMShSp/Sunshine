#pragma once


#include <iostream>
#include <vector>

#include "glm/glm.hpp"

#include "Vertex.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

#include "PerlinNoise.h"

#include "PlanetSettings.h"
#include "FacePlane.h"
#include "MinMax.h"


class PlanetFace
{
public:

	PlanetFace() = default;
	PlanetFace(int resolution, glm::vec3 updirection);
	PlanetFace(const int& resolution, const FacePlane& faceplane);

	void InitVertices();
	void InitIndices();
	void InitNormals();
	void SetNormal(const int& vertexindex, const glm::vec3& normal);
	void InitMesh();
	void SetPlanetSettings(PlanetSettings& settings);
	void UpdatePlanet();

	inline size_t GetIndexSize()
	{
		return m_indices.size();

	};
	inline size_t GetVertexSize()
	{
		return m_vertices.size();
	};
	inline int GetResolution()
	{
		return m_resolution;
	};

	std::vector<Vertex>& GetVertices()
	{

		return m_vertices;
	
	};
	VertexArrayLayout& GetLayout()
	{
		return m_layout;
	}

	MinMax& GetMinMax()
	{
		return m_minmax;
	}


	VertexBuffer m_vb;
	VertexArray m_vao;
	IndexBuffer m_ib;



private:

	glm::vec3 CubeToSphere(glm::vec3& cubepos);

	float Max(const float& firstval, const float& secondval);

	glm::vec3 m_updirection = glm::vec3(0.f,1.f,0.f);
	int m_resolution = 0;
	glm::vec3 m_axisA = glm::vec3(0.f);
	glm::vec3 m_axisB = glm::vec3(0.f);


	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;


	VertexArrayLayout m_layout;

	PerlinNoise m_perlin;

	PlanetSettings m_settings;

	MinMax m_minmax;

};