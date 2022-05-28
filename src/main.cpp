// Project includes
#include "ArgumentsChecker.h"
#include "ArgumentsParser.h"
#include "DatFileUnpacker.h"

// Third party includes

// stdlib
#include <iostream>

using namespace DatUnpacker;

void usage()
{
    std::cout << "Unpacker for Fallout 1/2 DAT files" << std::endl;
    std::cout << "v0.0.5 (c) 2012-2022 Falltergeist Developers" << std::endl;
    std::cout << "Usage: dat-unpacker [arguments]" << std::endl;
    std::cout << "Example: dat-unpacker -f dat1 -s ~/fallout1/master.dat -d ~/unpacked" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  --format, -f        Fallout DAT file format version. 'dat1' or 'dat2'. 'dat2' is default" << std::endl;
    std::cout << "                      Possible values are: 'dat1', 'dat2'." << std::endl;
    std::cout << "                      (Defaults to 'dat2')" << std::endl;
    std::cout << "  --quiet, -q         Quite mode. Do not display anything" << std::endl;
    std::cout << "  --transform, -t     Transform file names to lowercase" << std::endl;
    std::cout << "  --source, -s        Path to the DAT file" << std::endl;
    std::cout << "  --destination, -d   Where to extract files" << std::endl;
}

int main(int argc, char** argv)
{
    ArgumentsParser argumentsParser;
    auto arguments = argumentsParser.parse(argc, argv);

    ArgumentsChecker argumentsChecker;
    if (!argumentsChecker.check(arguments)) {
        if (!arguments.quietMode) {
            std::cerr << argumentsChecker.getErrorMessage() << std::endl << std::endl;
            usage();
        }
        return 1;
    }

    DatFileUnpacker datFileUnpacker;
    if (!datFileUnpacker.unpack(arguments)) {
        if (!arguments.quietMode) {
            std::cerr << datFileUnpacker.getErrorMessage() << std::endl << std::endl;
            usage();
        }
        return 1;
    }

    return 0;
}
