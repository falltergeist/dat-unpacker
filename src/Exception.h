#pragma once
// Project includes

// Third party includes

// stdlib
#include <exception>
#include <string>

namespace DatUnpacker
{
    class Exception : std::exception
    {
        public:
            explicit Exception(const char* message);
            explicit Exception(const std::string& message);
            virtual ~Exception() throw();
            virtual const char* what() const throw();
        private:
            std::string _message;
    };
}