/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2020 Falltergeist Developers
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

#ifndef DATUNPACKER_DATFILE_H
#define DATUNPACKER_DATFILE_H

// C++ standard includes
#include <vector>
#include <string>
#include <fstream>

// DatUnpacker includes

// Third party includes

namespace DatUnpacker
{
    class DatFileItem;

    class DatFile
    {
        public:
            enum class Endianness {
                Little, 
                Big
            };

            enum VERSION
            {
                FALLOUT1,
                FALLOUT2
            };

            DatFile(std::string filename, bool write = false);
            ~DatFile();

            std::string name();
            unsigned int version();
            void setVersion(unsigned int value);
            int64_t position();
            void setPosition(int64_t position);
            int64_t size();
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

        protected:
            int _version;
            Endianness _endianness;
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
    };
}
#endif // DATUNPACKER_DATFILE_H
