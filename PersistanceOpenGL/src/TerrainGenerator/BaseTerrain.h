#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>
#include <string>
#include <vector>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexArrayLayout.h"
#include "Shader.h"
#include "Array2D.h"



class BaseTerrain
{
public:
	BaseTerrain()
	{
	};

	//getters
	Array2D GetHeightMap() const { return m_heightmap; };
	


	//setters
	void SetSize(const unsigned int& xzsize);

	void SetShader(const Shader& shader);

	void SetHeightMap(const Array2D& heightmap);

	void InitHeightMap();

	void InitMesh();

	void UpdateHeight();

	void UpdateNormals();

	void Draw();

	std::vector<Vertex> GetVertices()
	{
		return m_vertices;

	}

	


	
private:

	void InitVertices(const int& meshsize);

	void InitIndices(const int& pointamount);


	unsigned int m_xzsize = -1;


	Shader m_shader;
	std::vector<Vertex>m_vertices;
	std::vector<unsigned int> m_indices;
	VertexBuffer m_vb;
	IndexBuffer m_ib;
	VertexArray m_vao;
	VertexArrayLayout m_layout;
	Array2D m_heightmap;





};

