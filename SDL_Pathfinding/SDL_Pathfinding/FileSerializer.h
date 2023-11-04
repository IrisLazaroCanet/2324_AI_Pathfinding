#pragma once
#include <map>
#include <string>

#define FS FileSerializer::GetInstance()

class FileSerializer
{
private:
	static FileSerializer* instance;
	FileSerializer();

public:
	static FileSerializer* GetInstance();

	void WriteMapToBinaryFile(const char* path, std::map<std::string, int> map);
};