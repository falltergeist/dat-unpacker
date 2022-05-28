#pragma once

// Project includes
#include "Arguments.h"

// Third party includes

// stdlib

namespace DatUnpacker
{
    class ArgumentsParser
    {
        public:
            ArgumentsParser() = default;
            Arguments parse(int argc, char** argv) const;
    };
}