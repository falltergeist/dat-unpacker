#pragma once
// Project includes

// Third party includes

// stdlib
#include <string>
#include <vector>

namespace DatUnpacker
{
    class FileSystemHelper
    {
        public:
            FileSystemHelper() = default;
            bool isExist(const std::string& path) const;
            bool isFile(const std::string& path) const;
            bool isDirectory(const std::string& path) const;
            bool createDirectory(const std::string& path, unsigned mode);
            bool createDirectoryRecursive(const std::string& path, unsigned mode);
            std::vector<std::string> splitPath(const std::string& path);
    };
}
