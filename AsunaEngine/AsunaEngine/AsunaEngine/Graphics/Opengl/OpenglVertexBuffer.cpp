#include "OpenglVertexBuffer.h"
#include "glad/glad.h"
using namespace asuna;

void asuna::OpenglVertexBuffer::Bind()
{
	// Enable the two vertex array attributes.
	glEnableVertexAttribArray(0);  // Vertex position.
	glEnableVertexAttribArray(1);  // Vertex color.

	// Specify the location and format of the position portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, 0);

	// Specify the location and format of the color portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 6, (char*)NULL + (3 * sizeof(float)));
}

OpenglVertexBuffer* OpenglVertexBuffer::Create(VertexBufferCreateParam param)
{
	auto vertexBuffer = new OpenglVertexBuffer();

	// Generate an ID for the vertex buffer.
	glGenBuffers(1, &vertexBuffer->m_VertexBuffer);
	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, param.m_ElementCount * param.GetFormatSize(), param.m_VertexData, GL_STATIC_DRAW);

	return vertexBuffer;
}
