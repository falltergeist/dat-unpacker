// Project includes
#include "ArgumentsChecker.h"

// Third party includes

// stdlib
#include <filesystem>
#include <system_error>

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
