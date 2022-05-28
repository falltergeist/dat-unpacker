#pragma once

// Project includes
#include "Format.h"

// Third party includes

// stdlib
#include <string>

namespace DatUnpacker
{
    typedef struct {
        bool quietMode = false;
        bool transformNames = false;
        std::string source;
        std::string destination;
        Format format = Format::FALLOUT2;
    } Arguments;
}