#pragma once
#include <memory>
#include <assimp/Importer.hpp>
#include "../Graphics/Abstract/Mesh.h"


namespace asuna
{
	class AssetLoader
	{
	public:
		static std::shared_ptr<MeshCreateParam> LoadMesh(const std::string& Path);
	private:
		static PrimitiveType ConvertPrimitiveType(unsigned int pt);
	private:
		static Assimp::Importer* MeshImporter;
	};


}
