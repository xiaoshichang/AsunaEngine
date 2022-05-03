#pragma once
#include <memory>
#include <assimp/Importer.hpp>

#include "../Graphics/Abstract/Mesh.h"
#include "../Graphics/Abstract/Texture.h"


namespace asuna
{
	class AssetLoader
	{
	public:
		static std::shared_ptr<MeshCreateParam> LoadMesh(const std::string& Path);
        static std::shared_ptr<RawTexture> LoadRawTexture(const std::string& path);
	private:
		static Assimp::Importer* MeshImporter;
	};


}
