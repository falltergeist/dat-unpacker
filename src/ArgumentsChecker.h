#pragma once

// Project includes
#include "Arguments.h"
#include "Format.h"

// Third party includes

// stdlib
#include <string>

namespace DatUnpacker
{
    class ArgumentsChecker
    {
        public:
            ArgumentsChecker() = default;
            bool check(const Arguments& arguments);
            bool checkFormat(const Format& format);
            bool checkSource(const std::string& source);
            bool checkDestination(const std::string& destination);
            std::string getErrorMessage() const;

        private:
            std::string _errorMessage;
    };
}
