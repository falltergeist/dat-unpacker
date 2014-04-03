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

#ifndef DATFILE_H
#define DATFILE_H

// C++ standard includes
#include <vector>
#include <string>
#include <fstream>

// Falltergeist includes

// Third party includes

class DatFileItem;

class DatFile
{
protected:
    int _version;
    unsigned int _endianness;
    std::ifstream _ifstream;
    std::ofstream _ofstream;
    std::string _filename;
    std::vector<DatFileItem*>* _items;
    unsigned int _swap(unsigned int value);
    int _swap(int value);
    unsigned short _swap(unsigned short value);
    short _swap(short value);
    void _fetchItems();
    DatFile& operator>>(unsigned int &value);
    DatFile& operator>>(int &value);
    DatFile& operator>>(unsigned short &value);
    DatFile& operator>>(short &value);
    DatFile& operator>>(unsigned char &value);
    DatFile& operator>>(char &value);
    DatFile& operator>>(std::string &value);

public:
    DatFile(std::string filename, bool write = false);
    ~DatFile();
    std::string name();
    unsigned int version();
    void setVersion(unsigned int value);
    int position();
    void setPosition(unsigned int position);
    int size();
    void skipBytes(unsigned int value);
    void readBytes(unsigned char* destination, unsigned int value);
    void writeBytes(unsigned char* source, unsigned int value);
    std::vector<DatFileItem*>* items();
    void setItems(std::vector<DatFileItem*>* value);
    DatFile& operator<<(unsigned int value);
    DatFile& operator<<(int value);
    DatFile& operator<<(unsigned short value);
    DatFile& operator<<(short value);
    DatFile& operator<<(unsigned char value);
    DatFile& operator<<(char value);
    DatFile& operator<<(std::string value);

};

#endif // DATFILE_H
