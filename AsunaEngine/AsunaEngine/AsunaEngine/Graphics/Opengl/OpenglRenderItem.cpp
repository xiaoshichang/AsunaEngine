#include "glad/glad.h"
#include "OpenglRenderItem.h"
#include "../../Foundation/Math/AMath.h"


using namespace asuna;

void OpenglRenderItem::Render()
{
	// Set the color shader as the current shader program and set the matrices that it will use for rendering.
	glUseProgram(m_ShaderProgram->m_Program);

	// set shader parameters
	{
		Vector3f up, position, lookAt;
		// Setup the vector that points upwards.
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;
		// Setup the position of the camera in the world.
		position.x = 10;
		position.y = 10;
		position.z = 10;
		// Setup where the camera is looking by default.
		lookAt.x = 0.0f;
		lookAt.y = 0.0f;
		lookAt.z = 0.0f;
		// open use col major matrix, so transpose matrix here.
		Matrix4x4f viewMatrix = BuildMatrixViewLookatRH(position, lookAt, up).TransposeCopy();
		// Set the field of view and screen aspect ratio.
		float fieldOfView, screenAspect;
		fieldOfView = PI / 4.0f;
		screenAspect = (float)1024 / (float)768;

		// Build the perspective projection matrix.
		Matrix4x4f projectionMatrix = BuildMatrixPerspectiveFovRH(fieldOfView, screenAspect, 0.1f, 1000.0f).TransposeCopy();
		Matrix4x4f worldMatrix = BuildMatrixIndentity();
		unsigned int location;
		// Set the world matrix in the vertex shader.
		location = glGetUniformLocation(m_ShaderProgram->m_Program, "worldMatrix");
		if (location == -1)
		{
			return;
		}
		glUniformMatrix4fv(location, 1, false, worldMatrix);

		// Set the view matrix in the vertex shader.
		location = glGetUniformLocation(m_ShaderProgram->m_Program, "viewMatrix");
		if (location == -1)
		{
			return;
		}
		glUniformMatrix4fv(location, 1, false, viewMatrix);

		// Set the projection matrix in the vertex shader.
		location = glGetUniformLocation(m_ShaderProgram->m_Program, "projectionMatrix");
		if (location == -1)
		{
			return;
		}
		glUniformMatrix4fv(location, 1, false, projectionMatrix);
	}
	
	// Bind the vertex array object that stored all the information about the vertex and index buffers.
	glBindVertexArray(m_Mesh->m_VertexArray);

	// Render the vertex buffer using the index buffer.
	glDrawElements(GL_TRIANGLES, m_Mesh->m_IndexBuffer->m_ElementCount, GL_UNSIGNED_SHORT, 0);
}
