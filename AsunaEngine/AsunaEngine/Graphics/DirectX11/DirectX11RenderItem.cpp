#include "../../Foundation/Math/AMath.h"
#include "DirectX11RenderItem.h"
#include "DirectX11RenderContext.h"
#include "DirectX11ConstantBuffer.h"
#include "DirectX11Shader.h"
#include "../Renderer.h"

using namespace asuna;
using namespace std;

void DirectX11RenderItem::Render()
{
	auto context = dynamic_pointer_cast<DirectX11RenderContext>(Renderer::Current->GetContext());
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
	screenAspect = Renderer::Current->m_ResolutionWidth / (float)Renderer::Current->m_ResolutionHeight;

	{
		auto cb = dynamic_pointer_cast<DirectX11ConstantBuffer>(GetConstantBuffer());
		auto data = cb->GetData();
		auto buffer = cb->GetBuffer();
		data.m_WorldMatrix = BuildMatrixRotationX(PI / 2);
		// directx use row major matrix, use dont transpose matrix here.
		data.m_ViexMatrix = BuildMatrixViewLookatLH(position, lookAt, up);
		data.m_ProjectionMatrix = BuildMatrixPerspectiveFovLH(fieldOfView, screenAspect, 0.1f, 1000.0f);
		// map data
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		auto result = context->m_DeviceContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		ASUNA_ASSERT(SUCCEEDED(result));
		auto dataPtr = (ConstantBufferData*)mappedResource.pData;
		memcpy(dataPtr, &data, sizeof(ConstantBufferData));
		context->m_DeviceContext->Unmap(buffer, 0);
		context->m_DeviceContext->VSSetConstantBuffers(0, 1, &buffer);
		context->m_DeviceContext->PSSetConstantBuffers(0, 1, &buffer);
	}


	auto vs = dynamic_pointer_cast<DirectX11VextexShader>(GetVertexShader());
	context->m_DeviceContext->IASetInputLayout(vs->GetLayout());
	context->m_DeviceContext->VSSetShader(vs->GetShader(), 0, 0);
	auto ps = dynamic_pointer_cast<DirectX11PixelShader>(GetPixelShader());
	context->m_DeviceContext->PSSetShader(ps->GetShader(), 0, 0);

	{
		auto mesh = GetMesh();
		for (int i = 0; i < mesh->m_SubMeshes.size(); i++)
		{
			auto subMesh = mesh->m_SubMeshes[i];
			auto positionBuffer = subMesh->GetPositionBuffer();
			auto normalBuffer = subMesh->GetNormalBuffer();
			auto texcoordBuffer = subMesh->GetTexcoordBuffer();
			auto indexBuffer = subMesh->GetIndexBuffer();

			if (positionBuffer != nullptr)
			{
				UINT stride = positionBuffer->GetStride();
				UINT offset = positionBuffer->GetOffset();
				auto vb = dynamic_pointer_cast<DirectX11VertexBuffer>(positionBuffer);
				auto buffer = vb->GetBuffer();
				// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetvertexbuffers
				context->m_DeviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
			}

			if (normalBuffer != nullptr)
			{
				UINT stride = normalBuffer->GetStride();
				UINT offset = normalBuffer->GetOffset();
				auto vb = dynamic_pointer_cast<DirectX11VertexBuffer>(normalBuffer);
				auto buffer = vb->GetBuffer();
				context->m_DeviceContext->IASetVertexBuffers(1, 1, &buffer, &stride, &offset);
			}

			if (texcoordBuffer != nullptr)
			{
				UINT stride = texcoordBuffer->GetStride();
				UINT offset = texcoordBuffer->GetOffset();
				auto vb = dynamic_pointer_cast<DirectX11VertexBuffer>(texcoordBuffer);
				auto buffer = vb->GetBuffer();
				context->m_DeviceContext->IASetVertexBuffers(2, 1, &buffer, &stride, &offset);
			}

			// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetprimitivetopology
			context->m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			auto ib = dynamic_pointer_cast<DirectX11IndexBuffer>(indexBuffer);
			// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-iasetindexbuffer
			context->m_DeviceContext->IASetIndexBuffer(ib->GetBuffer(), ib->GetDXGIFormat(), 0);

			// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-id3d11devicecontext-drawindexed
			context->m_DeviceContext->DrawIndexed(ib->GetElementCount(), 0, 0);

		}
	}
}

std::shared_ptr<DirectX11RenderItem> asuna::DirectX11RenderItem::Create(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> vs, std::shared_ptr<Shader> ps, std::shared_ptr<ConstantBuffer> cb)
{
	return make_shared<DirectX11RenderItem>(mesh, vs, ps, cb);
}
