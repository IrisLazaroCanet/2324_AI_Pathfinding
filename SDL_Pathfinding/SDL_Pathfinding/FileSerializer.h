#pragma once
#include <map>
#include <string>
#include <fstream>

#define FS FileSerializer::GetInstance()

class FileSerializer
{
private:
	static FileSerializer* instance;
	FileSerializer();

public:
	static FileSerializer* GetInstance();

	void WriteMapToTextFile(const char* path, std::map<std::string, int> map, int instanceNumber);
};