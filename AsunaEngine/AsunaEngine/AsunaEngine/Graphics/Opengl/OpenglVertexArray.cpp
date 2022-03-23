
#include <cstdint>

#include "glad/glad.h"
#include "OpenglVertexArray.h"
using namespace asuna;


OpenglVertexArray* OpenglVertexArray::Create(void* vertexData, int vertexCount, void* indexData, int indexCount)
{
	auto vertexBuffer = new OpenglVertexBuffer();
	auto indexBuffer = new OpenglIndexBuffer();
	indexBuffer->m_IndexCount = indexCount;
	auto vertexArray = new OpenglVertexArray();
	vertexArray->m_VertexBuffer = vertexBuffer;
	vertexArray->m_IndexBuffer = indexBuffer;

	// Allocate an OpenGL vertex array object.
	glGenVertexArrays(1, &vertexArray->m_VertexArray);
	// Bind the vertex array object to store all the buffers and vertex attributes we create here.
	glBindVertexArray(vertexArray->m_VertexArray);

	// Generate an ID for the vertex buffer.
	glGenBuffers(1, &vertexBuffer->m_VertexBuffer);

	// Bind the vertex buffer and load the vertex (position and color) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float) * 6, vertexData, GL_STATIC_DRAW);

	// Enable the two vertex array attributes.
	glEnableVertexAttribArray(0);  // Vertex position.
	glEnableVertexAttribArray(1);  // Vertex color.

	// Specify the location and format of the position portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float) * 6, 0);

	// Specify the location and format of the color portion of the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VertexBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(float) * 6, (char*)NULL + (3 * sizeof(float)));

	// Generate an ID for the index buffer.
	glGenBuffers(1, &indexBuffer->m_IndexBuffer);

	// Bind the index buffer and load the index data into it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->m_IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint16_t), indexData, GL_STATIC_DRAW);

	return vertexArray;
}
