#include "../../Foundation/Platform/Assert.h"
#include "../../3rd/Glad/include/glad/glad.h"
#include "OpenglIndexBuffer.h"

using namespace asuna;
using namespace std;

OpenglIndexBuffer::~OpenglIndexBuffer()
{
	if (m_VEO != 0)
	{
		glDeleteBuffers(1, &m_VEO);
		m_VEO = 0;
	}
}

unsigned short OpenglIndexBuffer::GetGLIndexType()
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VEO);
}

 shared_ptr<OpenglIndexBuffer> OpenglIndexBuffer::Create(shared_ptr<IndexBufferCreateParam> param)
{
	auto indexBuffer = make_shared<OpenglIndexBuffer>();
	indexBuffer->m_Format = param->m_Format;
	indexBuffer->m_ElementCount = param->m_ElementCount;

	// Generate an ID for the index buffer.
	glGenBuffers(1, &indexBuffer->m_VEO);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_VEO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, param->m_ElementCount * param->GetFormatSize(), param->m_IndexData, GL_STATIC_DRAW);
	return indexBuffer;
}
