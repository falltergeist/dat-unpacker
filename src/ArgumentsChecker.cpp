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
#include <filesystem>
#include <system_error>

// DatUnpacker includes
#include "ArgumentsChecker.h"

// Third party includes

namespace DatUnpacker
{
    bool ArgumentsChecker::check(const Arguments& arguments)
    {
        if (!checkFormat(arguments.format)) {
            return false;
        }

        if (!checkSource(arguments.source)) {
            return false;
        }

        if (!checkDestination(arguments.destination)) {
            return false;
        }

        return true;
    }

    std::string ArgumentsChecker::getErrorMessage() const
    {
        return _errorMessage;
    }

    bool ArgumentsChecker::checkFormat(const Format &format)
    {
        if (format == Format::UNKNOWN) {
            _errorMessage = "Unknown DAT file format version";
            return false;
        }
        return true;
    }

    bool ArgumentsChecker::checkSource(const std::string& source)
    {
        if (source.length() == 0) {
            _errorMessage = "Path to DAT file not specified";
            return false;
        }

        if (!std::filesystem::is_regular_file(source)) {
            _errorMessage = "DAT file not found: " + source;
            return false;
        }

        return true;
    }

    bool ArgumentsChecker::checkDestination(const std::string& destination)
    {
        if (destination.length() == 0) {
            _errorMessage = "Destination path not specified";
            return false;
        }

        if (!std::filesystem::is_directory(destination)) {
            std::error_code errc;
            std::filesystem::create_directories(destination, errc);

            if (errc) {
                _errorMessage = "Can't create destination directory: " + destination 
                    + " (" + errc.message() + ")";
                return false;
            }
        }
        return true;
    }
}
