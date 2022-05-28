#pragma once

// Project includes
#include "Format.h"

// Third party includes

// stdlib
#include <string>

namespace DatUnpacker
{
    struct Arguments {
        bool quietMode = false;
        bool transformNames = false;
        std::string source;
        std::string destination;
        Format format = Format::FALLOUT2;
    };
}