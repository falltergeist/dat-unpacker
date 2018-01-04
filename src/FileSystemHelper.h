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

#ifndef DATUNPACKER_FILESYSTEMHELPER_H
#define DATUNPACKER_FILESYSTEMHELPER_H

// C++ standard includes
#include <string>
#include <vector>

// DatUnpacker includes

// Third party includes

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

#endif //DATUNPACKER_FILESYSTEMHELPER_H
