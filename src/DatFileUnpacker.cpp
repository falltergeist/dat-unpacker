// Project includes
#include "DatFile.h"
#include "DatFileItem.h"
#include "DatFileUnpacker.h"
#include "FileSystemHelper.h"
#include "Arguments.h"

// Third party includes

// stdlib
#include <algorithm>
#include <iostream>

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

        FileSystemHelper fileSystemHelper;

        // extract items
        for (auto item : *datFile.items()) {

            std::string name = item->name();
            std::replace(name.begin(), name.end(), '\\', '/');

            if (arguments.transformNames) {
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            }

            std::string fullpath = arguments.destination + "/" + name;
            std::string directory = fullpath.substr(0, fullpath.find_last_of('/'));
            fileSystemHelper.createDirectoryRecursive(directory, 0755);

            if (!arguments.quietMode) {
                std::cout << fullpath << std::endl;
            }

            std::ofstream stream;
            stream.open(fullpath.c_str());
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

    bool DatFileUnpacker::checkDatFile(const DatFile& datFile, const Arguments& arguments)
    {
        // TODO check file integrity
        return true;
    }
}
