#pragma once
// Project includes

// Third party includes

// stdlib

namespace DatUnpacker
{
    class LZSS
    {
        public:
            static void decompress(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputSize);
    };
}