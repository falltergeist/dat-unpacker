/*
 * Copyright 2012-2014 Falltergeist Developers.
 *
 * This file is part of Falltergeist.
 *
 * Falltergeist is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Falltergeist is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Falltergeist.  If not, see <http://www.gnu.org/licenses/>.
 */

// C++ standard includes
#include <iostream>
#include <cstring>

// Falltergeist includes
#include "LZSS.h"

// Third party includes

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
                    if (F & 1 != 0)
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

                        for (unsigned int j = 0; j < L+3; j++)
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
