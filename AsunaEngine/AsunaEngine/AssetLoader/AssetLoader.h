#pragma once
#include <memory>
#include <assimp/Importer.hpp>
#include "../Graphics/Mesh.h"
#include "../DLLExport/DLLExport.h"


namespace asuna
{
	class ASUNAENGINE_API AssetLoader
	{
	public:
		static std::shared_ptr<MeshCreateParam> LoadMesh(const std::string& Path);
	private:
		static bool CheckLeftHandRenderAPI();
		static PrimitiveType ConvertPrimitiveType(unsigned int pt);
	private:
		static Assimp::Importer* MeshImporter;
	};


}
