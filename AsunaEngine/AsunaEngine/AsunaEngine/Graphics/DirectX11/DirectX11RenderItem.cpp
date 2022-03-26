#include "../../Foundation/Math/AMath.h"
#include "DirectX11RenderItem.h"
#include "DirectX11RenderContext.h"
#include "../Renderer.h"

using namespace asuna;

void DirectX11RenderItem::Render()
{
	auto context = (DirectX11RenderContext*)Renderer::Current->GetContext();
	Vector3f up, position, lookAt;
	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	// Setup the position of the camera in the world.
	position.x = 0;
	position.y = 00;
	position.z = 60;
	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 0.0f;
	float fieldOfView, screenAspect;
	fieldOfView = PI / 4.0f;
	screenAspect = (float)1024 / (float)768;

	// just invert z-axis to convert left hand to right hand coordinate
	m_ConstantBuffer->m_Data.m_WorldMatrix = BuildMatrixRotationX(PI / 2);
	// directx use row major matrix, use dont transpose matrix here.
	m_ConstantBuffer->m_Data.m_ViexMatrix = BuildMatrixViewLookatLH(position, lookAt, up);
	m_ConstantBuffer->m_Data.m_ProjectionMatrix = BuildMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);

	// map data
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		auto result = context->m_DeviceContext->Map(m_ConstantBuffer->m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		ASUNA_ASSERT(SUCCEEDED(result));
		auto dataPtr = (ConstantBufferData*)mappedResource.pData;
		memcpy(dataPtr, &m_ConstantBuffer->m_Data, sizeof(ConstantBufferData));
		context->m_DeviceContext->Unmap(m_ConstantBuffer->m_ConstantBuffer, 0);
		context->m_DeviceContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer->m_ConstantBuffer);
		context->m_DeviceContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer->m_ConstantBuffer);
	}

	context->m_DeviceContext->IASetInputLayout(m_VertexShader->m_Layout);
	context->m_DeviceContext->VSSetShader(m_VertexShader->m_VS, 0, 0);
	context->m_DeviceContext->PSSetShader(m_PixelShader->m_PS, 0, 0);

	for (int i = 0; i < m_Mesh->m_SubMeshes.size(); i++)
	{
		auto subMesh = m_Mesh->m_SubMeshes[i];

		if (subMesh->m_PositionBuffer != nullptr)
		{
			UINT stride = subMesh->m_PositionBuffer->m_Stride;
			UINT offset = subMesh->m_PositionBuffer->m_Offset;
			auto vb = (DirectX11VertexBuffer*)subMesh->m_PositionBuffer;
			// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
			context->m_DeviceContext->IASetVertexBuffers(0, 1, &vb->m_Buffer, &stride, &offset);
		}

		if (subMesh->m_NormalBuffer != nullptr)
		{
			UINT stride = subMesh->m_NormalBuffer->m_Stride;
			UINT offset = subMesh->m_NormalBuffer->m_Offset;
			auto vb = (DirectX11VertexBuffer*)subMesh->m_NormalBuffer;
			context->m_DeviceContext->IASetVertexBuffers(1, 1, &vb->m_Buffer, &stride, &offset);
		}

		if (subMesh->m_TexCoordBuffer != nullptr)
		{
			UINT stride = subMesh->m_TexCoordBuffer->m_Stride;
			UINT offset = subMesh->m_TexCoordBuffer->m_Offset;
			auto vb = (DirectX11VertexBuffer*)subMesh->m_TexCoordBuffer;
			context->m_DeviceContext->IASetVertexBuffers(2, 1, &vb->m_Buffer, &stride, &offset);
		}

		// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetprimitivetopology
		context->m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		auto indexBuffer = (DirectX11IndexBuffer*)subMesh->m_IndexBuffer;
		// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetindexbuffer
		context->m_DeviceContext->IASetIndexBuffer(indexBuffer->m_IndexBuffer, indexBuffer->GetDXGIFormat(), 0);

		// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-drawindexed
		context->m_DeviceContext->DrawIndexed(indexBuffer->m_ElementCount, 0, 0);

	}
}
