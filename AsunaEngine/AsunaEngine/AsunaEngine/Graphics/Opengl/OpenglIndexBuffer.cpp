#include "../../Foundation/Platform/Assert.h"
#include "glad/glad.h"
#include "OpenglIndexBuffer.h"

using namespace asuna;

unsigned short asuna::OpenglIndexBuffer::GetGLIndexType()
{
	if (m_Format == IndexBufferFormat::UINT16)
	{
		return GL_UNSIGNED_SHORT;
	}
	else if (m_Format == IndexBufferFormat::UINT32)
	{
		return GL_UNSIGNED_INT;
	}
	else
	{
		ASUNA_ASSERT(false);
		return 0;
	}
}

void OpenglIndexBuffer::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
}

OpenglIndexBuffer* OpenglIndexBuffer::Create(IndexBufferCreateParam* param)
{
	auto indexBuffer = new OpenglIndexBuffer();
	indexBuffer->m_Format = param->m_Format;
	indexBuffer->m_ElementCount = param->m_ElementCount;

	// Generate an ID for the index buffer.
	glGenBuffers(1, &indexBuffer->m_IndexBuffer);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, param->m_ElementCount * param->GetFormatSize(), param->m_IndexData, GL_STATIC_DRAW);
	return indexBuffer;
}
