// Project includes
#include "FileSystemHelper.h"

// Third party includes

// stdlib
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace DatUnpacker
{
    bool FileSystemHelper::isExist(const std::string& path) const
    {
        return isDirectory(path) || isFile(path);
    }

    bool FileSystemHelper::isFile(const std::string& path) const
    {
        std::ifstream stream;
        stream.open(path.c_str(), std::ios::binary);
        return stream.is_open();
    }

    bool FileSystemHelper::isDirectory(const std::string& path) const
    {
        struct stat info;
        if (stat(path.c_str(), &info) != 0) {
            // Destination does not exist
            return false;
        }
        if (!(info.st_mode & S_IFDIR)) {
            // Destination is not a directory
            return false;
        }
        return true;
    }

    bool FileSystemHelper::createDirectory(const std::string& path, unsigned mode)
    {
        if (isDirectory(path)) {
            return true;
        }
        #ifdef __unix__
            return mkdir(path.c_str(), mode) == 0;
        #endif
        return false;
    }

    bool FileSystemHelper::createDirectoryRecursive(const std::string& path, unsigned mode)
    {
        std::string pathToCreate;
        for (auto part : splitPath(path)) {
            pathToCreate += "/" + part;
            if (!createDirectory(pathToCreate, mode)) {
                return false;
            }
        }
        return true;
    }

    std::vector<std::string> FileSystemHelper::splitPath(const std::string& path)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(path);
        while (std::getline(tokenStream, token, '/')) {
            if (!token.empty()) {
                tokens.push_back(token);
            }
        }
        return tokens;
    }
}
