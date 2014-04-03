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

#ifndef DATFILEITEM_H
#define DATFILEITEM_H

// C++ standard includes
#include <string>

// Falltergeist includes

// Third party includes

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
