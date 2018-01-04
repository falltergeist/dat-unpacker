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

// DatUnpacker includes
#include "DatFileItem.h"
#include "DatFile.h"
#include "LZSS.h"

// Third party includes
#include "zlib.h"

namespace DatUnpacker
{
    DatFileItem::DatFileItem(DatFile* datFile)
    {
        _datFile = datFile;
        _packedSize = 0;
        _unpackedSize = 0;
        _compressed = false;
        _dataOffset = 0;
        _packedData = 0;
        _unpackedData = 0;
    }

    DatFileItem::~DatFileItem()
    {
        delete [] _packedData; _packedData = 0;
        delete [] _unpackedData; _unpackedData = 0;

    }

    std::string DatFileItem::name()
    {
        return _name;
    }

    unsigned int DatFileItem::packedSize()
    {
        return _packedSize;
    }

    unsigned int DatFileItem::unpackedSize()
    {
        return _unpackedSize;
    }

    unsigned int DatFileItem::dataOffset()
    {
        return _dataOffset;
    }

    bool DatFileItem::compressed()
    {
        return _compressed;
    }

    DatFileItem* DatFileItem::setName(std::string value)
    {
        _name = value;
        return this;
    }

    DatFileItem* DatFileItem::setPackedSize(unsigned int value)
    {
        _packedSize = value;
        return this;
    }

    DatFileItem* DatFileItem::setUnpackedSize(unsigned int value)
    {
        _unpackedSize = value;
        return this;
    }

    DatFileItem* DatFileItem::setDataOffset(unsigned int value)
    {
        _dataOffset = value;
        return this;
    }

    DatFileItem* DatFileItem::setCompressed(bool value)
    {
        _compressed = value;
        return this;
    }

    unsigned char* DatFileItem::data()
    {
        if (_unpackedData) return _unpackedData;

        _unpackedData = new unsigned char[_unpackedSize];
        _datFile->setPosition(_dataOffset);

        // If compressed
        if (!_compressed)
        {
            _datFile->readBytes(_unpackedData, _unpackedSize);
            return _unpackedData;
        }

        // If not compressed
        _packedData = new unsigned char[_packedSize];
        _datFile->readBytes(_packedData, _packedSize);
        switch(_datFile->version())
        {
            case 1:
            {
                LZSS::decompress(_packedData, _packedSize, _unpackedData, _unpackedSize);
                break;
            }
            case 2:
            {
                // unpacking
                z_stream zStream;
                zStream.total_in  = zStream.avail_in  = _packedSize;
                zStream.avail_in = _packedSize;
                zStream.next_in  = _packedData;
                zStream.total_out = zStream.avail_out = _unpackedSize;
                zStream.next_out = _unpackedData;
                zStream.zalloc = Z_NULL;
                zStream.zfree = Z_NULL;
                zStream.opaque = Z_NULL;
                inflateInit( &zStream );
                inflate( &zStream, Z_FINISH );
                inflateEnd( &zStream );
                break;
            }
        }
        delete [] _packedData; _packedData = 0;

        return _unpackedData;
    }

    std::string DatFileItem::extension()
    {
        return _name.substr(_name.length() - 3);
    }

    void DatFileItem::freeData()
    {
        delete [] _packedData; _packedData = 0;
        delete [] _unpackedData; _unpackedData = 0;
    }
}
