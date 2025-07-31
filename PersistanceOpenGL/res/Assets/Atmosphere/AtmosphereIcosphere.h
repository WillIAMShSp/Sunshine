#pragma once
#include "VertexArray.h"
#include "Shader.h"
#include <vector>


class AtmosphereIcosphere
{
public:
	AtmosphereIcosphere() = default;
	AtmosphereIcosphere(int resolution);





private:

	Shader m_shader;


	std::vector<Vertex> m_vertices;

};