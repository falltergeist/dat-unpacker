#pragma once
// Project includes

// Third party includes

// stdlib
#include <string>

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