// Project includes
#include "ArgumentsParser.h"
#include "Arguments.h"

// Third party includes

// stdlib
#include <string>

namespace DatUnpacker
{
    Arguments ArgumentsParser::parse(int argc, char** argv) const
    {
        Arguments arguments;

        for (int i = 0; i != argc; i++) {
            std::string argument = argv[i];

            if (argument == "--format" || argument == "-f") {
                arguments.format = Format::UNKNOWN;
                if (i < argc - 1) {
                    std::string format = argv[i+1];
                    if (format == "dat1") {
                        arguments.format = Format::FALLOUT1;
                    } else if (format == "dat2") {
                        arguments.format = Format::FALLOUT2;
                    }
                    i++;
                }
                continue;
            }

            if (argument == "--quiet" || argument == "-q") {
                arguments.quietMode = true;
                continue;
            }

            if (argument == "--transform" || argument == "-t") {
                arguments.transformNames = true;
                continue;
            }


            if ((argument == "--source" || argument == "-s") && (i < argc-1)) {
                arguments.source = argv[++i];
                continue;
            }

            if ((argument == "--destination" || argument == "-d") && (i < argc-1)) {
                arguments.destination = argv[++i];
                // rtrim
                arguments.destination.erase(arguments.destination.find_last_not_of("/\\") + 1);
                continue;
            }
        }

        return arguments;
    }
}
