/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2020 Falltergeist Developers
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
#include <algorithm>
#include <iostream>
#include <filesystem>

// DatUnpacker includes
#include "DatFile.h"
#include "DatFileItem.h"
#include "DatFileUnpacker.h"
#include "Arguments.h"

// Third party includes

namespace DatUnpacker
{
    bool DatFileUnpacker::unpack(const Arguments& arguments)
    {
        DatFile datFile(arguments.source);

        //if (arguments.format == Format::FALLOUT2) {
        //    datFile.setVersion(DatFile::VERSION::FALLOUT2);
        //} else {
        //    datFile.setVersion(DatFile::VERSION::FALLOUT1);
        //}

        // TODO: refactor DatFile to get rid of this call
        if (datFile.version()) {

        }

        if (!checkDatFile(datFile, arguments)) {
            return false;
        }

        // extract items
        for (auto item : *datFile.items()) {

            std::string name = item->name();
            std::replace(name.begin(), name.end(), '\\', '/');

            if (arguments.transformNames) {
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            }

            std::string fullpath = arguments.destination + "/" + name;
            std::string directory = fullpath.substr(0, fullpath.find_last_of('/'));

            std::filesystem::create_directories(directory);

            if (!arguments.quietMode) {
                std::cout << fullpath << std::endl;
            }

            std::ofstream stream;
            stream.open(fullpath.c_str(), std::ofstream::out | std::ofstream::binary);
            stream.write((char*)item->data(), item->unpackedSize());
            stream.close();
            delete [] item->data();
        }

        return true;
    }

    std::string DatFileUnpacker::getErrorMessage() const
    {
        return _errorMessage;
    }

    bool DatFileUnpacker::checkDatFile(const DatFile&, const Arguments&)
    {
        // TODO check file integrity
        return true;
    }
}
