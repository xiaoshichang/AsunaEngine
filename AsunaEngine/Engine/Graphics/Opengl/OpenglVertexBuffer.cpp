#include "OpenglVertexBuffer.h"
#include "../../3rd/Glad/include/glad/glad.h"
using namespace std;
using namespace asuna;

asuna::OpenglVertexBuffer::~OpenglVertexBuffer()
{
	if (m_VBO != 0)
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}
}

void OpenglVertexBuffer::Bind(int index)
{
	// Enable the two vertex array attributes.
	glEnableVertexAttribArray(index);

	// Specify the location and format of the position portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(index, 3, GL_FLOAT, false, GetStride(), 0);
}

shared_ptr<OpenglVertexBuffer> OpenglVertexBuffer::Create(shared_ptr<VertexBufferCreateParam> param)
{
	// Generate an ID for the vertex buffer.
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	auto dataSize = param->GetFormatStride() * param->m_ElementCount;
	glBufferData(GL_ARRAY_BUFFER, dataSize, param->m_VertexData, GL_STATIC_DRAW);

	return make_shared<OpenglVertexBuffer>(vbo, param->m_Format, param->m_ElementCount, param->GetFormatStride(), 0);
}
