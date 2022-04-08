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
	ASUNA_ASSERT(sceneRoot);

	param->m_SubMeshCount = sceneRoot->mNumMeshes;
	for (size_t i = 0; i < sceneRoot->mNumMeshes; i++)
	{
		auto meshNode = sceneRoot->mMeshes[i];
		auto subParam = make_shared<SubMeshCreateParam>();
		if (meshNode->HasPositions())
		{
			subParam->m_PositionCreateParam = make_shared<VertexBufferCreateParam>();
			subParam->m_PositionCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_PositionCreateParam->m_ElementCount = meshNode->mNumVertices;
			subParam->m_PositionCreateParam->m_VertexData = meshNode->mVertices;
		}

		if (meshNode->HasNormals())
		{
			subParam->m_NormalCreateParam = make_shared<VertexBufferCreateParam>();
			subParam->m_NormalCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_NormalCreateParam->m_ElementCount = meshNode->mNumVertices;
			subParam->m_NormalCreateParam->m_VertexData = meshNode->mNormals;
		}

		if (meshNode->HasTextureCoords(0))
		{
			subParam->m_TexcoordCreateParam = make_shared<VertexBufferCreateParam>();
			subParam->m_TexcoordCreateParam->m_Format = VertexBufferFormat::F3;
			subParam->m_TexcoordCreateParam->m_ElementCount = meshNode->mNumVertices;
			subParam->m_TexcoordCreateParam->m_VertexData = meshNode->mTextureCoords[0];
		}
		subParam->m_PrimitiveType = ConvertPrimitiveType(meshNode->mPrimitiveTypes);
		subParam->m_IndexCreateParam = make_shared<IndexBufferCreateParam>();
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
		param->m_SubMeshCreateParam.push_back(subParam);
	}

	return param;
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
