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
	void AddBufferInstancedMat4(VertexBuffer& vb);
	void SetLayout(VertexArrayLayout& layout);

	inline uint16_t& GetAmountofElements()
	{
		return m_totalelements;
	}


	void Bind();
	void UnBind();


private:

	unsigned int m_RendererID;
	uint16_t m_totalelements = 0;



};

