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
