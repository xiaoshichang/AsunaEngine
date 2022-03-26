#include <iostream>

#include "AssetLoader.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Foundation/Platform/Assert.h"
#include "../Graphics/Renderer.h"

using namespace asuna;
using namespace Assimp;

Assimp::Importer* AssetLoader::MeshImporter = new Assimp::Importer();

/// <summary>
/// http://assimp.sourceforge.net/lib_html/data.html
/// By default, all 3D data is provided in a right-handed coordinate system such as OpenGL uses. 
/// In this coordinate system, +X points to the right, -Z points away from the viewer into the screen and +Y points upwards.
/// </summary>
MeshCreateParam* AssetLoader::LoadMesh(const std::string& scenePath)
{
	MeshCreateParam* param = new MeshCreateParam();

	auto readFlag = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
	if (CheckLeftHandRenderAPI())
	{
		readFlag = readFlag | aiProcess_ConvertToLeftHanded;
	}

	auto sceneRoot = MeshImporter->ReadFile(scenePath, readFlag);
	ASUNA_ASSERT(sceneRoot);

	param->m_SubMeshCount = sceneRoot->mNumMeshes;
	param->m_SubMeshCreateParam = new SubMeshCreateParam[param->m_SubMeshCount];
	for (size_t i = 0; i < sceneRoot->mNumMeshes; i++)
	{
		auto meshNode = sceneRoot->mMeshes[i];
		auto subParam = &(param->m_SubMeshCreateParam[i]);
		if (meshNode->HasPositions())
		{
			subParam->m_PositionCreateParam = new VertexBufferCreateParam();
			subParam->m_PositionCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_PositionCreateParam->m_ElementCount = meshNode->mNumVertices;
			subParam->m_PositionCreateParam->m_VertexData = meshNode->mVertices;
		}

		if (meshNode->HasNormals())
		{
			subParam->m_NormalCreateParam = new VertexBufferCreateParam();
			subParam->m_NormalCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_NormalCreateParam->m_ElementCount = meshNode->mNumVertices;
			subParam->m_NormalCreateParam->m_VertexData = meshNode->mNormals;
		}

		if (meshNode->HasTextureCoords(0))
		{
			subParam->m_TexcoordCreateParam = new VertexBufferCreateParam();
			subParam->m_TexcoordCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_TexcoordCreateParam->m_ElementCount = meshNode->mNumVertices;
			subParam->m_TexcoordCreateParam->m_VertexData = meshNode->mTextureCoords[0];
		}
		subParam->m_PrimitiveType = ConvertPrimitiveType(meshNode->mPrimitiveTypes);
		subParam->m_IndexCreateParam = new IndexBufferCreateParam();
		subParam->m_IndexCreateParam->m_Format = IndexBufferFormat::UINT32;

		if (meshNode->mPrimitiveTypes == aiPrimitiveType::aiPrimitiveType_TRIANGLE)
		{
			int elementCount = meshNode->mNumFaces * 3;
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
	}

	return param;
}

bool AssetLoader::CheckLeftHandRenderAPI()
{
	switch (Renderer::Current->GetRenderAPIType())
	{
	case RenderAPIType::Directx11:
	case RenderAPIType::Directx12:
		return true;
	case RenderAPIType::Opengl:
	case RenderAPIType::Opengles:
		return false;
	default:
		ASUNA_ASSERT(false);
		return false;
	}
}

PrimitiveType AssetLoader::ConvertPrimitiveType(unsigned int pt)
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
