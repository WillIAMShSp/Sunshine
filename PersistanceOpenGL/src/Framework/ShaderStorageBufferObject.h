#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Vertex.h"
#include "glm/glm.hpp"


class ShaderStorageBufferObject
{

public:
	ShaderStorageBufferObject();

	~ShaderStorageBufferObject();

	void Bind(const uint32_t& slot = 0);

	void UnBind();

	void AddBuffer(const size_t& datasize, const void* data);

	void AddBuffer(std::vector<glm::mat4>& matrices);


private:


	unsigned int m_RendererID = -1;

	uint32_t m_slot = -1;




};