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

#ifndef DATUNPACKER_DATFILEITEM_H
#define DATUNPACKER_DATFILEITEM_H

// C++ standard includes
#include <string>

// DatUnpacker includes

// Third party includes

namespace DatUnpacker
{
    class DatFile;

    class DatFileItem
    {
        public:
            DatFileItem(DatFile* datFile);
            ~DatFileItem();
            std::string name();
            std::string extension();
            unsigned int packedSize();
            unsigned int unpackedSize();
            bool compressed();
            unsigned int dataOffset();
            unsigned char* data();
            void freeData();
            DatFileItem* setPackedSize(unsigned int value);
            DatFileItem* setUnpackedSize(unsigned int value);
            DatFileItem* setDataOffset(unsigned int value);
            DatFileItem* setCompressed(bool value);
            DatFileItem* setName(std::string value);
        protected:
            DatFile* _datFile;
            unsigned char* _packedData;
            unsigned char* _unpackedData;
            std::string _name;
            unsigned int _packedSize;
            unsigned int _unpackedSize;
            bool _compressed;
            unsigned int _dataOffset;
    };
}
#endif // DATUNPACKER_DATFILEITEM_H
