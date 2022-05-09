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

    Vector3f normalData[4] =
    {
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}
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

    subParam->m_NormalCreateParam = make_shared<VertexBufferCreateParam>();
    subParam->m_NormalCreateParam->m_Format = VertexBufferFormat::F3;
    subParam->m_NormalCreateParam->m_ElementCount = 4;
    subParam->m_NormalCreateParam->m_VertexData = normalData;

    subParam->m_IndexCreateParam = make_shared<IndexBufferCreateParam>();
    subParam->m_IndexCreateParam->m_ElementCount = 6;
    subParam->m_IndexCreateParam->m_Format = IndexBufferFormat::UINT32;
    subParam->m_IndexCreateParam->m_StartIndex = 0;
    subParam->m_IndexCreateParam->m_IndexData = indexData;

    param->m_SubMeshCreateParam.push_back(subParam);
    param->m_MaterialCount = 1;
    auto mesh = Renderer::Instance->CreateMesh(param);
    return mesh;
}

std::shared_ptr<Mesh> SimpleGeometryCreator::CreateCube()
{
    Vector3f positionData[24] =
    {
            // top
            {0.5f, 0.5f, 0.5f},
            {0.5f, 0.5f, -0.5f},
            {-0.5f, 0.5f, -0.5f},
            {-0.5f, 0.5f, 0.5f},
            // bottom
            {0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, -0.5f},
            {-0.5f, -0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f},
            // left
            {-0.5f, 0.5f, 0.5f},
            {-0.5f, -0.5f, 0.5f},
            {-0.5f, -0.5f, -0.5f},
            {-0.5f, 0.5f, -0.5f},
            // right
            {0.5f, 0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f},
            // front
            {-0.5f, 0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f},
            {-0.5f, -0.5f, -0.5f},
            // back
            {-0.5f, 0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f},
            {-0.5f, -0.5f, 0.5f},

    };

    Vector3f normalData[24] =
    {
            // top
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f},
            // bottom
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f},
            // left
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f},
            // right
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f},
            // front
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f},
            // back
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f},
    };

    unsigned int indexData[36] = {
            0, 1, 2, 0, 2, 3, 4, 6, 5, 4, 7, 6,
            8, 11, 10, 8, 10, 9, 12, 14, 15, 12, 13, 14,
            16, 17, 18, 16, 18, 19, 20, 22, 21, 20, 23, 22
    };

    auto param = make_shared<MeshCreateParam>();
    auto subParam = make_shared<SubMeshCreateParam>();
    subParam->m_ModelMatrix = Matrix4x4f::Identity();
    subParam->m_MaterialIndex = 0;
    subParam->m_PrimitiveType = PrimitiveType::Triangle;

    subParam->m_PositionCreateParam = make_shared<VertexBufferCreateParam>();
    subParam->m_PositionCreateParam->m_Format = VertexBufferFormat::F3;
    subParam->m_PositionCreateParam->m_ElementCount = 24;
    subParam->m_PositionCreateParam->m_VertexData = positionData;

    subParam->m_NormalCreateParam = make_shared<VertexBufferCreateParam>();
    subParam->m_NormalCreateParam->m_Format = VertexBufferFormat::F3;
    subParam->m_NormalCreateParam->m_ElementCount = 24;
    subParam->m_NormalCreateParam->m_VertexData = normalData;

    subParam->m_IndexCreateParam = make_shared<IndexBufferCreateParam>();
    subParam->m_IndexCreateParam->m_ElementCount = 36;
    subParam->m_IndexCreateParam->m_Format = IndexBufferFormat::UINT32;
    subParam->m_IndexCreateParam->m_StartIndex = 0;
    subParam->m_IndexCreateParam->m_IndexData = indexData;

    param->m_SubMeshCreateParam.push_back(subParam);
    param->m_MaterialCount = 1;
    auto mesh = Renderer::Instance->CreateMesh(param);
    return mesh;
}

