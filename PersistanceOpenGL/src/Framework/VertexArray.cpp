#include "VertexArray.h"

#include <iostream>



VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
	glBindVertexArray(m_RendererID);
	std::cout << "Constructing Vertex Array \n";
}

VertexArray::~VertexArray()
{
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexArrayLayout& layout)
{
	Bind();
	vb.Bind();

	unsigned int stride = layout.GetStride();

	auto& elements = layout.GetElements();

	
	int multiplier = 1;

	int offset = 0;

	for (int i = 0; i < elements.size(); i++)
	{
		

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, stride, (const void*)(offset*sizeof(elements[i].type)));

		offset += elements[i].count;

	}

}

void VertexArray::AddBufferInstanced(const uint32_t& attributelocation, VertexBuffer& vb, VertexArrayLayout& layout)
{


	Bind();
	vb.Bind();

	unsigned int stride = layout.GetStride();

	auto& elements = layout.GetElements();


	int multiplier = 1;

	int offset = 0;


	uint32_t selectedattribute = attributelocation;

	for (int i = 0; i < elements.size(); i++)
	{


		glEnableVertexAttribArray(selectedattribute + i);
		glVertexAttribPointer(selectedattribute + i, elements[i].count, elements[i].type, elements[i].normalized, stride, (const void*)(offset * sizeof(elements[i].type)));

		offset += elements[i].count;

		glVertexAttribDivisor(selectedattribute+i, 1);

	}





}

void VertexArray::AddBufferInstancedMat4(VertexBuffer& vb)
{

	Bind();
	vb.Bind();

	std::size_t vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);







}

void VertexArray::SetLayout(VertexArrayLayout& layout)
{
	Bind();
	

	unsigned int stride = layout.GetStride();

	auto& elements = layout.GetElements();


	int multiplier = 1;

	int offset = 0;

	for (int i = 0; i < elements.size(); i++)
	{


		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, stride, (const void*)(offset * sizeof(elements[i].type)));

		offset += elements[i].count;

	}
}

void VertexArray::Bind()
{
	glBindVertexArray(m_RendererID);


}

void VertexArray::UnBind()
{

	glBindVertexArray(0);
}
