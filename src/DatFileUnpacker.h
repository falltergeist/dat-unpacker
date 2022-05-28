#pragma once

// Project includes
#include "Arguments.h"

// Third party includes

// stdlib
#include <string>

namespace DatUnpacker
{
    class DatFile;

    class DatFileUnpacker
    {
        public:
            DatFileUnpacker() = default;
            bool unpack(const Arguments& arguments);
            std::string getErrorMessage() const;
            bool checkDatFile(const DatFile& datFile, const Arguments& arguments);

        private:
            std::string _errorMessage;
    };
}