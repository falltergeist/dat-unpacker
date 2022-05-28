// Project includes
#include "ArgumentsChecker.h"
#include "FileSystemHelper.h"

// Third party includes

// stdlib

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

        FileSystemHelper fileSystemHelper;
        if (!fileSystemHelper.isFile(source)) {
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

        FileSystemHelper fileSystemHelper;

        if (!fileSystemHelper.isDirectory(destination)) {
            if (!fileSystemHelper.createDirectoryRecursive(destination, 0755)) {
                _errorMessage = "Can't create destination directory: " + destination;
                return false;
            }
            throw 1;
        }

        return true;
    }
}
