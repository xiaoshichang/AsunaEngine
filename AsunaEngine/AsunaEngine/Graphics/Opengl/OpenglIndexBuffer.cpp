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
	unsigned int veo;
	// Generate an ID for the index buffer.
	glGenBuffers(1, &veo);
	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, param->m_ElementCount * param->GetFormatSize(), param->m_IndexData, GL_STATIC_DRAW);
	return make_shared<OpenglIndexBuffer>(veo, param->m_ElementCount, param->m_Format);
}
