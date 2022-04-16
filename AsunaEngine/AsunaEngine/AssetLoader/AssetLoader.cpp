#include <iostream>
#include "AssetLoader.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Foundation/Platform/Assert.h"
#include "../Graphics/Abstract/Renderer.h"

using namespace asuna;
using namespace Assimp;
using namespace std;

Assimp::Importer* AssetLoader::MeshImporter = new Assimp::Importer();


Matrix4x4f ConvertMatrix(const aiMatrix4x4& matrix)
{
    Matrix4x4f ret;
    for (int r = 0; r < 4; ++r)
    {
        for (int c = 0; c < 4; ++c)
        {
            ret[r][c] = matrix[r][c];
        }
    }
    return ret;
}

PrimitiveType ConvertPrimitiveType(unsigned int pt)
{
    if (pt == aiPrimitiveType::aiPrimitiveType_POINT)
    {
        return PrimitiveType::Point;
    }
    else if (pt == aiPrimitiveType::aiPrimitiveType_LINE)
    {
        return PrimitiveType::Line;
    }
    else if (pt == aiPrimitiveType::aiPrimitiveType_TRIANGLE)
    {
        return PrimitiveType::Triangle;
    }
    else
    {
        ASUNA_ASSERT(false);
        return PrimitiveType::Unknown;
    }
}

void VisitAssimpSceneNodeToLoadMesh(const aiScene* scene, const aiNode* node, aiMatrix4x4 parentTransform, const std::shared_ptr<MeshCreateParam>& param)
{
    for (int i = 0; i < node->mNumMeshes; ++i)
    {
        auto meshNode = scene->mMeshes[node->mMeshes[i]];
        auto modelMatrix = parentTransform * node->mTransformation;
        auto subParam = make_shared<SubMeshCreateParam>();
        if (meshNode->HasPositions())
		{
			subParam->m_PositionCreateParam = make_shared<VertexBufferCreateParam>();
			subParam->m_PositionCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_PositionCreateParam->m_ElementCount = (int)meshNode->mNumVertices;
			subParam->m_PositionCreateParam->m_VertexData = meshNode->mVertices;
		}

		if (meshNode->HasNormals())
		{
			subParam->m_NormalCreateParam = make_shared<VertexBufferCreateParam>();
			subParam->m_NormalCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_NormalCreateParam->m_ElementCount = (int)meshNode->mNumVertices;
			subParam->m_NormalCreateParam->m_VertexData = meshNode->mNormals;
		}

		if (meshNode->HasTextureCoords(0))
		{
			subParam->m_TexcoordCreateParam = make_shared<VertexBufferCreateParam>();
			subParam->m_TexcoordCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_TexcoordCreateParam->m_ElementCount = (int)meshNode->mNumVertices;
			subParam->m_TexcoordCreateParam->m_VertexData = meshNode->mTextureCoords[0];
		}
		subParam->m_PrimitiveType = ConvertPrimitiveType(meshNode->mPrimitiveTypes);
		subParam->m_IndexCreateParam = make_shared<IndexBufferCreateParam>();
		subParam->m_IndexCreateParam->m_Format = IndexBufferFormat::UINT32;

		if (meshNode->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE)
		{
			int elementCount = (int)meshNode->mNumFaces * 3;
			subParam->m_IndexCreateParam->m_IndexData = new unsigned int[elementCount];
			subParam->m_IndexCreateParam->m_ElementCount = elementCount;
			for (size_t j = 0; j < meshNode->mNumFaces; j++)
			{
				auto face = meshNode->mFaces[j];
				auto ptr = (unsigned int*)(subParam->m_IndexCreateParam->m_IndexData);
				ptr[j * 3] = face.mIndices[0];
				ptr[j * 3 + 1] = face.mIndices[1];
				ptr[j * 3 + 2] = face.mIndices[2];
			}
		}
        else
        {
            ASUNA_ASSERT(false);
        }
        subParam->m_MaterialIndex = (int)meshNode->mMaterialIndex;
        subParam->m_ModelMatrix = ConvertMatrix(modelMatrix);
		param->m_SubMeshCreateParam.push_back(subParam);
    }

    for (int i = 0; i < node->mNumChildren; ++i)
    {
        auto child = node->mChildren[i];
        VisitAssimpSceneNodeToLoadMesh(scene, child, parentTransform * node->mTransformation, param);
    }
}


/// <summary>
/// http://assimp.sourceforge.net/lib_html/data.html
/// By default, all 3D data is provided in a right-handed coordinate system such as OpenGL uses. 
/// In this coordinate system, +X points to the right, -Z points away from the viewer into the screen and +Y points upwards.
/// </summary>
shared_ptr<MeshCreateParam> AssetLoader::LoadMesh(const std::string& Path)
{
	auto param = make_shared<MeshCreateParam>();
	auto readFlag = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
	if (Renderer::Current->CheckLeftHandRenderAPI())
	{
		readFlag = readFlag | aiProcess_ConvertToLeftHanded;
	}

	auto sceneRoot = MeshImporter->ReadFile(Path, readFlag);
    param->m_RawResource = sceneRoot;
    param->m_MaterialCount = sceneRoot->mNumMaterials;
	ASUNA_ASSERT(sceneRoot);
    VisitAssimpSceneNodeToLoadMesh(sceneRoot, sceneRoot->mRootNode, aiMatrix4x4(), param);
	return param;
}



