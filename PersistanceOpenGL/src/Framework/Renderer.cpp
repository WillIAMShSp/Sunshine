#include "Renderer.h"



Renderer::Renderer()
{


}

void Renderer::Draw(VertexArray& vao, IndexBuffer& ib, Shader& shader)
{
	
	vao.Bind();
	ib.Bind();
	shader.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

}

void Renderer::Draw(VertexArray& vao, size_t count,  Shader& shader)
{
	vao.Bind();
	shader.Bind();

	glDrawArrays(GL_TRIANGLES, 0, count);




}

void Renderer::DrawInstanced(VertexArray& vao, size_t count, Shader& shader, const uint32_t& amountofinstances)
{
	vao.Bind();
	shader.Bind();

	glDrawArraysInstanced(GL_TRIANGLES, 0, count, amountofinstances);

}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}
