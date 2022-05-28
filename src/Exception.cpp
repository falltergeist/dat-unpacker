// Project includes
#include "Exception.h"

// Third party includes

// stdlib

namespace DatUnpacker
{
    Exception::Exception(const char* message) : _message(message)
    {
    }

    Exception::Exception(const std::string& message) : _message(message)
    {
    }

    Exception::~Exception() throw()
    {
    }

    const char* Exception::what() const throw()
    {
        return _message.c_str();
    }
}
