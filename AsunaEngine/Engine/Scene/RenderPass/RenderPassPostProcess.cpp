//
// Created by xiao on 2022/5/7.
//

#include "RenderPassPostProcess.h"
#include "../../Foundation/Math/AMath.h"
#include "../../Graphics/Abstract/Renderer.h"
using namespace asuna;
using namespace std;

RenderPassPostProcess::RenderPassPostProcess()
{
    CreateMaterial();
    CreateRenderItem();
}


void RenderPassPostProcess::Render(const shared_ptr<RenderTarget> &inputRT, const shared_ptr<RenderTarget> &outputRT)
{
    Renderer::Instance->SetRenderTarget(outputRT);
    Renderer::Instance->ClearRenderTarget(outputRT, 0.1f, 0.2f, 0.3f, 1.0f);

    m_Material->SetTexture("MainTex", inputRT);
    m_RenderItem->Render(m_Material);
}

void RenderPassPostProcess::CreateRenderItem() {

    Vector3f positionData[4] =
    {
        {-1, -1, 0},
        {-1, 1, 0},
        {1, -1, 0},
        {1, 1, 0}
    };

    // directx texture coord
    // (0, 0)
    // |------------|
    // |            |
    // |            |
    // |            |
    // |------------| (1, 1)
    Vector3f TexcoordDataLeft[4] =
    {
        {0, 1, 0},
        {0, 0, 0},
        {1, 1, 0},
        {1, 0, 0}
    };

    // opengl texture coord
    // |------------| (1, 1)
    // |            |
    // |            |
    // |            |
    // |------------|
    // (0, 0)
    Vector3f TexcoordDataRight[4] =
    {
            {0, 0, 0},
            {0, 1, 0},
            {1, 0, 0},
            {1, 1, 0}
    };

    unsigned int indexDataLeft[6] = {0, 1, 2, 2, 1, 3};
    unsigned int indexDataRight[6] = {2, 1, 0, 3, 1, 2};

    auto param = make_shared<MeshCreateParam>();
    auto subParam = make_shared<SubMeshCreateParam>();
    subParam->m_PositionCreateParam = make_shared<VertexBufferCreateParam>();
    subParam->m_PositionCreateParam->m_ElementCount = 4;
    subParam->m_PositionCreateParam->m_Format = VertexBufferFormat::F3;
    subParam->m_PositionCreateParam->m_VertexData = positionData;

    subParam->m_TexcoordCreateParam = make_shared<VertexBufferCreateParam>();
    subParam->m_TexcoordCreateParam->m_ElementCount = 4;
    subParam->m_TexcoordCreateParam->m_Format = VertexBufferFormat::F3;

    subParam->m_IndexCreateParam = make_shared<IndexBufferCreateParam>();
    subParam->m_IndexCreateParam->m_Format = IndexBufferFormat::UINT32;
    subParam->m_IndexCreateParam->m_ElementCount = 6;

    if (Renderer::Instance->CheckLeftHandRenderAPI())
    {
        subParam->m_IndexCreateParam->m_IndexData = indexDataLeft;
        subParam->m_TexcoordCreateParam->m_VertexData = TexcoordDataLeft;

    }
    else
    {
        subParam->m_IndexCreateParam->m_IndexData = indexDataRight;
        subParam->m_TexcoordCreateParam->m_VertexData = TexcoordDataRight;
    }

    subParam->m_PrimitiveType = PrimitiveType::Triangle;

    param->m_SubMeshCreateParam.push_back(subParam);
    m_Mesh = Renderer::Instance->CreateMesh(param);
    m_ConstantBuffer = Renderer::Instance->CreateConstantBuffer(ConstantBufferDataType::PerObject, sizeof(ConstantBufferDataPerObject));
    m_RenderItem = Renderer::Instance->CreateRenderItem(m_Mesh, m_ConstantBuffer);
}

void RenderPassPostProcess::CreateMaterial()
{
    m_Material = Renderer::Instance->CreateMaterial("Global_PostProcess", MaterialType::PostProcess);
}


