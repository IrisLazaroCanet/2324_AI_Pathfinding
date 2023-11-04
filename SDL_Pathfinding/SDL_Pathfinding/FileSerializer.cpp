#include "FileSerializer.h"

FileSerializer* FileSerializer::instance = nullptr;

FileSerializer::FileSerializer()
{

}

FileSerializer* FileSerializer::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new FileSerializer();
    }

    return instance;
}

void FileSerializer::WriteMapToBinaryFile(const char* path, std::map<std::string, int> map)
{
    //TODO: Save data in file
}
