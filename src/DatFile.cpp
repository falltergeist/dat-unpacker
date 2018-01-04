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
#include <algorithm>

// DatUnpacker includes
#include "DatFile.h"
#include "DatFileItem.h"

// Third party includes

namespace DatUnpacker
{
    DatFile::DatFile(std::string filename, bool write)
    {
        _filename = filename;
        _endianness = LITTLE_ENDIAN;
        _version = -1;
        _items = 0;

        if (write)
        {
            _ofstream.open(_filename.c_str(), std::ios::binary | std::ios::trunc);
            if (!_ofstream.is_open())
            {
                throw 1;
            }
        }
    }

    DatFile::~DatFile()
    {
        if (_ifstream.is_open()) _ifstream.close();
        if (_ofstream.is_open()) _ofstream.close();
        delete _items;
    }

    unsigned int DatFile::_swap(unsigned int value)
    {
        return (value << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | (value >> 24);
    }

    int DatFile::_swap(int value)
    {
        return _swap((unsigned int ) value);
    }

    unsigned short DatFile::_swap(unsigned short value)
    {
        return (value << 8) | (value >> 8);
    }

    short DatFile::_swap(short value)
    {
        return _swap((unsigned short) value);
    }

    void DatFile::setVersion(unsigned int value)
    {
        _version = value;
        if (_version == 1)
        {
            _endianness = BIG_ENDIAN;
        }
    }

    unsigned int DatFile::version()
    {
        if (_version >=0) return _version;

        try
        {
            _version = 2;
            _fetchItems();
        }
        catch(...)
        {
            try
            {
                _version = 1;
                _fetchItems();
            }
            catch(...)
            {
                _version = 0;
            }
        }
        return _version;
    }

    void DatFile::_fetchItems()
    {
        if (_items) return;

        if (!_ifstream.is_open())
        {
            _ifstream.open(_filename.c_str(), std::ios::binary);
            if (!_ifstream.is_open())
            {
                throw 1;
            }
        }

        unsigned int realSize = size();

        switch(_version)
        {
            case 1:
            {
                //fetching items
                _endianness = BIG_ENDIAN;
                setPosition(0);
                unsigned int directoriesCounter;
                std::vector<std::string> directories;

                *this >> directoriesCounter;
                skipBytes(3*4);

                // reading directories
                for (unsigned int i = 0; i != directoriesCounter; ++i)
                {
                    unsigned char length;
                    std::string name;
                    *this >> length;
                    name.resize(length);
                    *this >> name;
                    directories.push_back(name);
                }
                // reading files
                _items = new std::vector<DatFileItem*>;
                for (unsigned int i = 0; i != directoriesCounter; ++i)
                {
                    unsigned int filesCounter;
                    *this >> filesCounter;
                    skipBytes(3*4);
                    for (unsigned int j = 0; j != filesCounter; ++j)
                    {
                        unsigned char length;
                        std::string name;
                        unsigned int compression, packedSize, unpackedSize, dataOffset;
                        DatFileItem* item = new DatFileItem(this);

                        *this >> length;
                        name.resize(length);
                        *this >> name >> compression >> dataOffset >> unpackedSize >> packedSize;

                        if (directories.at(i) != ".")
                        {
                            name = directories.at(i) + "/" + name;
                        }
                        // Replace slashes and transform to lower case
                        //std::replace(name.begin(),name.end(),'\\','/');
                        //std::transform(name.begin(),name.end(),name.begin(), ::tolower);

                        item->setName(name)
                            ->setCompressed(compression == 0x20 ? false : true)
                            ->setDataOffset(dataOffset)
                            ->setUnpackedSize(unpackedSize)
                            ->setPackedSize(packedSize);
                        _items->push_back(item);
                    }
                }
                break;
            }
            case 2:
            {
                // checking size signature
                if (realSize < 12) throw 1;
                setPosition(realSize - 4);
                unsigned int size;
                *this >> size;
                if (realSize != size) throw 1;

                // fetching items
                setPosition(realSize - 8);
                unsigned int filesTreeSize;
                *this >> filesTreeSize;
                //std::cout << "FilesTreeSize: " << filesTreeSize << std::endl;

                setPosition(realSize - filesTreeSize - 8);
                unsigned int filesCounter;
                *this >> filesCounter;
                //std::cout << "FilesCounter: " << filesCounter << std::endl;

                _items = new std::vector<DatFileItem*>;
                for (unsigned int i = 0; i != filesCounter; ++i)
                {
                    unsigned int length;
                    std::string name;
                    unsigned char compressed;
                    unsigned int unpackedSize, packedSize, dataOffset;
                    DatFileItem* item = new DatFileItem(this);

                    //std::cout << "Pos: " << this->position() << std::endl;
                    *this >> length;
                    //std::cout << "Length: " << length << std::endl;
                    name.resize(length);
                    *this >> name >> compressed >> unpackedSize >> packedSize >> dataOffset;

                    // Replace slashes and transform to lower case
                    //std::replace(name.begin(),name.end(),'\\','/');
                    //std::transform(name.begin(),name.end(),name.begin(), ::tolower);

                    item->setName(name)
                        ->setUnpackedSize(unpackedSize)
                        ->setPackedSize(packedSize)
                        ->setCompressed((bool) compressed)
                        ->setDataOffset(dataOffset);
                    _items->push_back(item);
                }
                break;
            }
            default:
                throw 1;
        }
    }

    int DatFile::position()
    {
        if (_ifstream.is_open())
        {
            return _ifstream.tellg();
        }

        if (_ofstream.is_open())
        {
            return _ofstream.tellp();
        }

        return -1;
    }

    void DatFile::setPosition(unsigned int position)
    {
        if (_ifstream.is_open())
        {
            _ifstream.seekg(position, std::ios::beg);
            return;
        }

        if (_ofstream.is_open())
        {
            _ofstream.seekp(position, std::ios::beg);
            return;
        }
    }

    int DatFile::size()
    {
        int oldPosition = position();

        if (_ifstream.is_open())
        {
            _ifstream.seekg(0, std::ios::end);
        }

        if (_ofstream.is_open())
        {
            _ofstream.seekp(0, std::ios::end);
        }

        int value = position();
        setPosition(oldPosition);
        return value;
    }

    DatFile& DatFile::operator>>(unsigned int &value)
    {
        _ifstream.read((char*)&value, sizeof(unsigned int));
        if (_endianness == BIG_ENDIAN)
        {
            value = _swap(value);
        }
        return *this;
    }

    DatFile& DatFile::operator>>(int &value)
    {
        return operator>>((unsigned int&)value);
    }

    DatFile& DatFile::operator>>(unsigned short &value)
    {
        _ifstream.read((char*)&value, sizeof(unsigned short));
        if (_endianness == BIG_ENDIAN)
        {
            value = _swap(value);
        }
        return *this;
    }

    DatFile& DatFile::operator>>(short &value)
    {
        return operator>>((unsigned short&)value);
    }

    DatFile& DatFile::operator>>(unsigned char &value)
    {
        _ifstream.read((char*)&value, sizeof(unsigned char));
        return *this;
    }

    DatFile& DatFile::operator>>(char &value)
    {
        return operator>>((unsigned char&)value);
    }

    DatFile& DatFile::operator>>(std::string &value)
    {
        _ifstream.read(&value[0], value.size());
        return *this;
    }

    std::vector<DatFileItem*>* DatFile::items()
    {
        return _items;
    }

    void DatFile::skipBytes(unsigned int value)
    {
        setPosition(position() + value);
    }

    void DatFile::readBytes(unsigned char* destination, unsigned int value)
    {
        _ifstream.read((char*)destination, value);
    }

    std::string DatFile::name()
    {
        unsigned int pos = _filename.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            return _filename.substr(pos + 1);
        }
        return _filename;
    }

    void DatFile::setItems(std::vector<DatFileItem*>* value)
    {
        delete [] _items;
        _items = value;
    }

    DatFile& DatFile::operator<<(unsigned int value)
    {
        if (_endianness == BIG_ENDIAN)
        {
            value = _swap(value);
        }
        _ofstream.write((char*)&value, 4);
        _ofstream.flush();

        return *this;
    }

    DatFile& DatFile::operator<<(int value)
    {
        return  operator<<((unsigned int) value);
    }

    DatFile& DatFile::operator<<(unsigned short value)
    {
        if (_endianness == BIG_ENDIAN)
        {
            value = _swap(value);
        }
        _ofstream.write((char*)&value, 2);
        _ofstream.flush();

        return *this;
    }

    DatFile& DatFile::operator<<(short value)
    {
        return  operator<<((unsigned short) value);
    }

    DatFile& DatFile::operator<<(unsigned char value)
    {
        _ofstream.write((char*)&value, 1);
        _ofstream.flush();
        return *this;
    }

    DatFile& DatFile::operator<<(char value)
    {
        return  operator<<((unsigned char) value);
    }

    DatFile& DatFile::operator<<(std::string value)
    {
        _ofstream << value;
        _ofstream.flush();
        return *this;
    }

    void DatFile::writeBytes(unsigned char* source, unsigned int value)
    {
        _ofstream.write((char*)source, value);
        _ofstream.flush();
    }
}
