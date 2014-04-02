#include "DatFileItem.h"
#include "DatFile.h"
#include "LZSS.h"
#include <algorithm>
#include <iostream>
#include "zlib.h"

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
    // Replace slashes and transform to lower case
    std::replace(_name.begin(),_name.end(),'\\','/');
    std::transform(_name.begin(),_name.end(),_name.begin(), ::tolower);
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
