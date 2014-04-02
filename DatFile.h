#ifndef DATFILE_H
#define DATFILE_H

#include <vector>
#include <string>
#include <fstream>

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
    DatFile(std::string filename);
    ~DatFile();
    std::string name();
    unsigned int version();
    void setVersion(unsigned int value);
    int position();
    void setPosition(unsigned int position);
    int size();
    void skipBytes(unsigned int value);
    void readBytes(unsigned char* destination, unsigned int value);
    std::vector<DatFileItem*>* items();

};

#endif // DATFILE_H
