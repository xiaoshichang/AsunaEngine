//
// Created by xiao on 2022/4/19.
//

#include "SimpleGeometryCreator.h"
#include "../Graphics/Abstract/Renderer.h"
using namespace asuna;
using namespace std;

shared_ptr<Mesh> SimpleGeometryCreator::CreatePlane()
{
    Vector3f positionData[4] =
    {
        {1.0f, 0.0f, 1.0f},
        {1.0f, 0.0f, -1.0f},
        {-1.0f, 0.0f, -1.0f},
        {-1.0f, 0.0f, 1.0f},
    };

    unsigned int indexData[6] = {0, 1, 2, 0, 2, 3};

    auto param = make_shared<MeshCreateParam>();
    auto subParam = make_shared<SubMeshCreateParam>();
    subParam->m_ModelMatrix = Matrix4x4f::Identity();
    subParam->m_MaterialIndex = 0;
    subParam->m_PrimitiveType = PrimitiveType::Triangle;
    subParam->m_PositionCreateParam = make_shared<VertexBufferCreateParam>();
    subParam->m_PositionCreateParam->m_Format = VertexBufferFormat::F3;
    subParam->m_PositionCreateParam->m_ElementCount = 4;
    subParam->m_PositionCreateParam->m_VertexData = positionData;

    subParam->m_IndexCreateParam = make_shared<IndexBufferCreateParam>();
    subParam->m_IndexCreateParam->m_ElementCount = 6;
    subParam->m_IndexCreateParam->m_Format = IndexBufferFormat::UINT32;
    subParam->m_IndexCreateParam->m_StartIndex = 0;
    subParam->m_IndexCreateParam->m_IndexData = indexData;

    param->m_SubMeshCreateParam.push_back(subParam);
    param->m_MaterialCount = 1;
    auto mesh = Renderer::Current->CreateMesh(param);
    return mesh;
}

