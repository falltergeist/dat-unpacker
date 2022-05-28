#pragma once

// Project includes

// Third party includes

// stdlib
#include <vector>
#include <string>
#include <fstream>

namespace DatUnpacker
{
    class DatFileItem;

    class DatFile
    {
        public:
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
    };
}
