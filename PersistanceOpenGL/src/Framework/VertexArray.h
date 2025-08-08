#pragma once

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexArrayLayout.h"

class VertexArray 
{
public:

	VertexArray();

	~VertexArray();

	void AddBuffer(VertexBuffer& vb, VertexArrayLayout& layout);
	void AddBufferInstanced(const uint32_t& attributelocation, VertexBuffer& vb, VertexArrayLayout& layout);
	void SetLayout(VertexArrayLayout& layout);

	void Bind();
	void UnBind();


private:

	unsigned int m_RendererID;



};

