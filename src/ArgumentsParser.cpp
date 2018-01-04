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
#include <string>

// DatUnpacker includes
#include "ArgumentsParser.h"
#include "Arguments.h"

// Third party includes

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
