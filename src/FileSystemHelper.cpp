/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2018 Falltergeist Developers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// C++ standard includes
#include <fstream>
#include <sstream>
#include <sys/stat.h>

// DatUnpacker includes
#include "FileSystemHelper.h"

// Third party includes

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
