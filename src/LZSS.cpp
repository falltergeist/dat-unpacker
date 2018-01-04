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
#include <iostream>
#include <cstring>

// DatUnpacker includes
#include "LZSS.h"

// Third party includes

namespace DatUnpacker
{
    void  LZSS::decompress(unsigned char* input, unsigned int inputSize, unsigned char* output, unsigned int outputSize)
    {
        unsigned short D = 4096;
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
