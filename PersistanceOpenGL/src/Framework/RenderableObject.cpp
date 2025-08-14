#include "RenderableObject.h"

RenderableObject::RenderableObject()
{

}

RenderableObject::~RenderableObject()
{
}


void RenderableObject::SetPosition(const glm::vec3& newpos)
{
	m_position = newpos;

}

void RenderableObject::SetScale(const glm::vec3& newscale)
{

	m_scale = newscale;



}

void RenderableObject::SetInstancingParameters(const uint32_t& instanceamount, VertexBuffer& instancematrix, VertexArrayLayout& instancedarraylayout)
{
	m_instanceamount = instanceamount;

	m_model.SetInstancing(instancematrix, instancedarraylayout);


}

void RenderableObject::SetInstancingMat4(const uint32_t& instanceamount, VertexBuffer& instancematrix)
{
	m_instanceamount = instanceamount;
	m_model.SetInstancingMat4(instancematrix);


}

void RenderableObject::SetShader(const Shader& shader)
{
	m_shadersource = shader;

}

void RenderableObject::SetModel(const ModelLoader& model)
{
	m_model = model;

}

void RenderableObject::SetRenderer(const Renderer& renderer)
{
	m_renderer = renderer;
}

void RenderableObject::Draw()
{
	m_shadersource.Bind();
	m_modelmatrix = glm::mat4(1.f);

	m_modelmatrix = glm::translate(m_modelmatrix, m_position);
	m_modelmatrix = glm::scale(m_modelmatrix, m_scale);


	m_shadersource.SetUniformMat4fv("u_Model", m_modelmatrix);
	m_shadersource.SetUniform3f("u_instanceposition", m_position.x, m_position.y, m_position.z);


	m_model.Draw(m_renderer);

	m_shadersource.UnBind();


}

void RenderableObject::DrawInstanced(const uint32_t& instanceamount)
{
	if (instanceamount)
	{
		m_instanceamount = instanceamount;
	}

	m_shadersource.Bind();

	m_model.DrawInstanced(m_renderer,m_instanceamount);


	

}

