#include "Heart.h"

Heart::Heart()
{

}

Heart::~Heart()
{
}

void Heart::Update()
{
	Draw();

}

void Heart::SetPosition(const glm::vec3& newpos)
{
	m_position = newpos;

}

void Heart::SetShader(const Shader& shader)
{
	m_shadersource = shader;

}

void Heart::SetModel(const ModelLoader& model)
{
	m_model = model;

}

void Heart::SetRenderer(const Renderer& renderer)
{
	m_renderer = renderer;
}

void Heart::Draw()
{
	m_shadersource.Bind();
	m_modelmatrix = glm::mat4(1.f);

	m_modelmatrix = glm::translate(m_modelmatrix, m_position);



	m_shadersource.SetUniformMat4fv("u_Model", m_modelmatrix);
	m_shadersource.SetUniform3f("u_instanceposition", m_position.x, m_position.y, m_position.z);


	m_model.Draw(m_renderer);

	m_shadersource.UnBind();


}

