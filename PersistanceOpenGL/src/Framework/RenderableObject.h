#pragma once
#include "ModelLoader.h"
#include "MaterialLoader.h"
#include "Shader.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class RenderableObject
{
public:

	RenderableObject();
	~RenderableObject();


	
	void SetShader(const Shader& shader);
	void SetModel(const ModelLoader& model);
	void SetInstancingParameters(const uint32_t& instanceamount, VertexBuffer& instancematrix, VertexArrayLayout& instancedarraylayout);
	void SetInstancingMat4(const uint32_t& instanceamount, VertexBuffer& instancematrix);
	void SetRenderer(const Renderer& renderer);
	void Draw();
	void DrawInstanced(const uint32_t& instanceamount = false);










private: //Dependencies

	
	ModelLoader m_model;
	
	Shader m_shadersource;
	
	Renderer m_renderer;


	glm::mat4 m_modelmatrix = glm::mat4(1.f);
	glm::vec3 m_position = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 m_orientation = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 m_scale = glm::vec3(1.0);


	uint32_t m_instanceamount;



public:


	void SetPosition(const glm::vec3& newpos);
	void SetScale(const glm::vec3& newscale);
	



}; 
