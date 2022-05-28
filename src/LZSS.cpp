// Project includes
#include "LZSS.h"

// Third party includes

// stdlib
#include <iostream>
#include <cstring>

namespace DatUnpacker
{
    void  LZSS::decompress(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputSize)
    {
        const unsigned short D = 4096;
        unsigned short DO, DI = 0;
        short N;
        unsigned char dictionary[D];
        unsigned char F = 0;
        unsigned char L = 0;
        unsigned char* inputEnd = input + inputSize;
        unsigned char* outputEnd = output + outputSize;

        while (input < inputEnd)
        {

            N = *(input++) << 8;
            N |= *(input++);
            if (N == 0) return;

            if (N < 0)
            {
                unsigned char* end = input - N;
                while (input < end && output < outputEnd)
                {
                    unsigned char byte = *(input++);
                    *(output++) = byte;
                }
            }
            else
            {
                DO = D-18;
                memset(dictionary, ' ', D);
                unsigned char* end = input + N;
                while (input < end)
                {
                    F = *(input++);
                    for (unsigned int i = 0; i != 8 && input < end; ++i)
                    {
                        if ((F & 1) != 0)
                        {
                            unsigned char byte = *(input++);
                            *(output++) = byte;
                            dictionary[DO] = byte;
                            DO++;
                            if (DO >= D) DO = 0;
                        }
                        else
                        {
                            DI = *(input++);
                            L = *(input++);
                            DI = DI | ((0xF0 & L) << 4);
                            L &= 0x0F;

                            for (int j = 0; j < L+3; j++)
                            {
                                unsigned char byte = dictionary[DI];
                                *(output++) = byte;
                                dictionary[DO] = byte;
                                DI++;
                                DO++;
                                if (DO >= D) DO = 0;
                                if (DI >= D) DI = 0;
                            }
                        }
                        F >>= 1;
                    }
                }
            }
        }
    }
}
