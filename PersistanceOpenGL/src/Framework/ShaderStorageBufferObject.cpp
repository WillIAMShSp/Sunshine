#include "ShaderStorageBufferObject.h"

ShaderStorageBufferObject::ShaderStorageBufferObject()
{
	
	glGenBuffers(1, &m_RendererID);


}

ShaderStorageBufferObject::~ShaderStorageBufferObject()
{
}

void ShaderStorageBufferObject::Bind(const uint32_t& slot)
{
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	
	m_slot = slot;
}

void ShaderStorageBufferObject::UnBind()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	

}

void ShaderStorageBufferObject::AddBuffer(const size_t& datasize, const void* data)
{
	Bind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_slot, m_RendererID);

	glBufferData(GL_SHADER_STORAGE_BUFFER, datasize, data, GL_STATIC_DRAW);
	UnBind();

	
}

void ShaderStorageBufferObject::AddBuffer(std::vector<glm::mat4>& matrices)
{
	Bind();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_slot, m_RendererID);

	glBufferData(GL_SHADER_STORAGE_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);
	UnBind();

}


