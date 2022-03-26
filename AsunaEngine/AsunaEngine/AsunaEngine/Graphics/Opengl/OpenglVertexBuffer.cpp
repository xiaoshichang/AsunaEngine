#include "OpenglVertexBuffer.h"
#include "glad/glad.h"
using namespace std;
using namespace asuna;

void OpenglVertexBuffer::Bind(int index)
{
	// Enable the two vertex array attributes.
	glEnableVertexAttribArray(index);

	// Specify the location and format of the position portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glVertexAttribPointer(index, 3, GL_FLOAT, false, m_Stride, 0);
}

shared_ptr<OpenglVertexBuffer> OpenglVertexBuffer::Create(shared_ptr<VertexBufferCreateParam> param)
{
	auto vertexBuffer = make_shared<OpenglVertexBuffer>();
	vertexBuffer->m_Format = param->m_Format;
	vertexBuffer->m_ElementCount = param->m_ElementCount;
	vertexBuffer->m_Offset = 0;
	vertexBuffer->m_Stride = param->GetFormatStride();

	// Generate an ID for the vertex buffer.
	glGenBuffers(1, &vertexBuffer->m_VertexBuffer);
	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VertexBuffer);
	auto dataSize = vertexBuffer->m_Stride * vertexBuffer->m_ElementCount;
	glBufferData(GL_ARRAY_BUFFER, dataSize, param->m_VertexData, GL_STATIC_DRAW);

	return vertexBuffer;
}
