#pragma once
#include <string>
#include <fstream>
#include <streambuf>

namespace asuna
{
	class FileHelper
	{
	public:
		static std::string ReadText(const char* filePath)
		{
			std::ifstream in(filePath);
			std::string contents((std::istreambuf_iterator<char>(in)),
				std::istreambuf_iterator<char>());
			return contents;
		}
	};
}


