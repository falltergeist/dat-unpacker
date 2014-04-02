#ifndef DATFILEITEM_H
#define DATFILEITEM_H

#include <string>

class DatFile;

class DatFileItem
{
protected:
    DatFile* _datFile;
    unsigned char* _packedData;
    unsigned char* _unpackedData;
    std::string _name;
    unsigned int _packedSize;
    unsigned int _unpackedSize;
    bool _compressed;
    unsigned int _dataOffset;
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
};

#endif // DATFILEITEM_H
