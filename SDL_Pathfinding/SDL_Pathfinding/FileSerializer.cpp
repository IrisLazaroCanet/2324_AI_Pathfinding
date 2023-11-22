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

void FileSerializer::WriteMapToTextFile(const char* path, std::map<std::string, int> map, int instanceNumber)
{
    std::ofstream outputFile(path, std::ios::out | std::ios::app);
    if (outputFile.is_open())
    {
        outputFile << "Instance #";
        outputFile << instanceNumber;
        outputFile << '\n\n';
        for (std::map<std::string, int>::iterator it = map.begin(); it != map.end(); it++)
        {
            outputFile << it->first;
            outputFile << ' ';
            outputFile << it->second;
        }

        outputFile.close();
    }
}
