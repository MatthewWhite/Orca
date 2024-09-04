#include "Utils.h"

#include <cerrno>
#include <cstdio>

void* LoadFileContents(const std::string& filename)
{
    FILE* pFile = fopen(filename.c_str(), "rb");
    if (!pFile)
    {
        printf("Failed to load contents of file \"%s\". %s\n", filename.c_str(), strerror(errno));
        return nullptr;
    }

    std::fseek(pFile, 0, SEEK_END);
    size_t len = std::ftell(pFile);
    std::fseek(pFile, 0, SEEK_SET);

    char* pContents = new char[len + 1];
    std::fread(pContents, sizeof(char), len, pFile);
    std::fclose(pFile);
    pContents[len] = '\0';

    return pContents;
}