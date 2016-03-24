/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2012-2015 Falltergeist Developers
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
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <dirent.h>
#include <map>

// DatFile includes
#include "defines.h"
#include "DatFile.h"
#include "DatFileItem.h"

// Third party includes

using namespace DatFile;

struct
{
    bool quietMode = false;
    std::string action;
    std::string source;
    std::string destination;
    std::string error;
    Format format = Format::FALLOUT2;

} options;

DatFile::DatFile* datFile = 0;
std::map<std::string, std::vector<DatFileItem*>*> filesList;

void scanDirectory(std::string path)
{
    std::string shortPath = path.substr(options.source.length() + 1);
    if (shortPath != ".") shortPath = shortPath.substr(2);
    filesList.insert(std::pair<std::string, std::vector<DatFileItem*>*>(shortPath, new std::vector<DatFileItem*>));

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (path.c_str())) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            std::string name = ent->d_name;
            if (name == "." || name == "..") continue;
            struct stat s;
            std::string newpath = path + "/" + name;
            stat(newpath.c_str(), &s);
            if (s.st_mode & S_IFDIR)
            {
                //directories.push_back(newpath.substr(source.length() + 1));
                scanDirectory(newpath);
            }
            else
            {
                DatFileItem* item = new DatFileItem(0);
                item->setName(name);
                item->setPackedSize(s.st_size);
                item->setUnpackedSize(s.st_size);
                filesList.at(shortPath)->push_back(item);
            }
        }
        closedir (dir);
        if (filesList.at(shortPath)->size() == 0)
        {
            filesList.erase(shortPath);
        }
    }
    else
    {
        std::cout << "could not open directory: " << path << std::endl;
    }
}

bool checkDatFile()
{
    if (options.source.length() == 0)
    {
        options.error = "Source file is not defined";
        return false;
    }
    std::ifstream stream;
    stream.open(options.source.c_str(), std::ios::binary);
    if (!stream.is_open())
    {
        options.error = "Can't open source file: " + options.source;
        return false;
    }

    if (!datFile) datFile = new DatFile::DatFile(options.source);

    if (!datFile->version())
    {
        options.error = options.source + " is not a DAT file";
        return false;
    }
    return true;
}

bool actionPack()
{
    // source - folder
    // destination - file

    datFile = new DatFile::DatFile(options.destination, true);
    datFile->setVersion(options.format);
    datFile->setItems(new std::vector<DatFileItem*>);

    scanDirectory(options.source + "/.");

    if (options.format == Format::FALLOUT2)
    {
        for (std::map<std::string, std::vector<DatFileItem*>*>::iterator it = filesList.begin(); it != filesList.end(); ++it)
        {
            std::string directory = it->first;
            std::vector<DatFileItem*>* files = it->second;
            for (std::vector<DatFileItem*>::iterator it = files->begin(); it != files->end(); ++it)
            {
                DatFileItem* item = *it;
                std::string path = options.source + "/" + directory + "/" + item->name();

                std::cout << directory << "/" << item->name() << std::endl;

                item->setDataOffset(datFile->position());
                std::ifstream stream(path.c_str(), std::ios::binary | std::ios::in);
                unsigned char* buffer = new unsigned char[item->unpackedSize()];
                stream.read((char*)buffer, item->unpackedSize());
                datFile->writeBytes(buffer, item->unpackedSize());
                delete [] buffer;
                stream.close();

                std::string newName = directory + "/" + item->name();
                std::replace(newName.begin(),newName.end(),'/','\\');

                // Replace slashes and transform to lower case
                //std::replace(newName.begin(),newName.end(),'\\','/');
                //std::transform(newName.begin(),newName.end(),newName.begin(), ::tolower);
                item->setName(newName);

                datFile->items()->push_back(item);
            }
        }
        unsigned int dirTreePosition = datFile->position();

        unsigned int filesTotal = datFile->items()->size();
        *datFile << filesTotal;

        for (std::vector<DatFileItem*>::iterator it = datFile->items()->begin(); it != datFile->items()->end(); ++it)
        {
            DatFileItem* item = *it;
            unsigned char compression = 0;

            if (item->name()[0] == '.')
            {
                item->setName(item->name().substr(2));
            }


            *datFile << (unsigned int) item->name().length() << item->name() << compression << item->unpackedSize() << item->packedSize() << item->dataOffset();
        }

        unsigned int dirTreeSize = datFile->position() - dirTreePosition;
        *datFile << dirTreeSize << (unsigned int) (datFile->position() + 8);
    }
    else if (options.format == Format::FALLOUT1)
    {
        *datFile << (unsigned int) filesList.size() << (unsigned int) 0x0A << (unsigned int) 0x0 << (unsigned int) 0x0;
        unsigned int dataOffset = 16;
        for (std::map<std::string, std::vector<DatFileItem*>*>::iterator it = filesList.begin(); it != filesList.end(); ++it)
        {
            std::string directory = it->first;
            std::replace(directory.begin(),directory.end(),'/','\\');
            *datFile << (unsigned char) directory.length() << directory;
            dataOffset += 1 + directory.length();
        }

        // Calculating header size
        for (std::map<std::string, std::vector<DatFileItem*>*>::iterator it = filesList.begin(); it != filesList.end(); ++it)
        {
            dataOffset += 16;
            std::vector<DatFileItem*>* items = it->second;
            for (std::vector<DatFileItem*>::iterator it = items->begin(); it != items->end(); ++it)
            {
                dataOffset += 17 + (*it)->name().length();
            }
        }

        for (std::map<std::string, std::vector<DatFileItem*>*>::iterator it = filesList.begin(); it != filesList.end(); ++it)
        {
            std::vector<DatFileItem*>* items = it->second;
            *datFile << (unsigned int) items->size() << (unsigned int) 0x2E << (unsigned int) 0x10 << (unsigned int) 0x0;
            for (std::vector<DatFileItem*>::iterator it = items->begin(); it != items->end(); ++it)
            {
                DatFileItem* item = *it;
                *datFile << (unsigned char) item->name().length() << item->name();
                *datFile << (unsigned int) 0x20; // !NO COMPRESSION
                *datFile << dataOffset << item->unpackedSize() << (item->compressed() ? item->packedSize() : 0);
                dataOffset += item->packedSize();
            }
        }

        for (std::map<std::string, std::vector<DatFileItem*>*>::iterator it = filesList.begin(); it != filesList.end(); ++it)
        {
            std::string directory = it->first;
            std::vector<DatFileItem*>* items = it->second;
            for (std::vector<DatFileItem*>::iterator it = items->begin(); it != items->end(); ++it)
            {
                DatFileItem* item = *it;
                std::string path = options.source + "/" + directory + "/" + item->name();

                std::cout << directory << "/" << item->name() << std::endl;
                std::ifstream stream(path.c_str(), std::ios::binary | std::ios::in);
                unsigned char* buffer = new unsigned char[item->unpackedSize()];
                stream.read((char*)buffer, item->unpackedSize());
                datFile->writeBytes(buffer, item->unpackedSize());
                delete [] buffer;
                stream.close();
            }
        }


    }
    return true;
}

bool actionUnpack()
{
    if (!checkDatFile()) return false;

    // check destination folder
    if (options.destination.length() == 0)
    {
        options.error = "Destination directory is not defined";
        return false;
    }
    else
    {
        struct stat info;
        if(stat(options.destination.c_str(), &info) != 0)
        {
            options.error = "Destination directory does not exists: " + options.destination;
            return false;
        }
        if(!info.st_mode & S_IFDIR )
        {
            options.error = "Destination is not a directory: " + options.destination;
            return false;
        }
    }
    // extract items
    for (std::vector<DatFileItem*>::iterator it = datFile->items()->begin(); it != datFile->items()->end(); ++it)
    {
        DatFileItem* item = *it;

        // rtrim
        options.destination.erase(options.destination.find_last_not_of("/\\")+1);

        std::string name = item->name();
        std::replace(name.begin(),name.end(),'\\','/');
        //std::transform(name.begin(),name.end(),name.begin(), ::tolower);

        std::string basepath = options.destination + "/";
        std::string fullpath = basepath + name;
        std::string path = fullpath;
        std::string dirpath = "";
        while (path.find('/') != std::string::npos)
        {
            unsigned int pos = path.find('/');
            dirpath += path.substr(0, pos) + "/";
            path = path.substr(pos + 1);
            // create directory
            #ifdef __unix__
                mkdir(dirpath.substr(0, dirpath.length()-1).c_str(), 0755);
            #endif

        }

        if (!options.quietMode) std::cout << fullpath << std::endl;
        std::ofstream stream;
        stream.open(fullpath.c_str());
        stream.write((char*)item->data(), item->unpackedSize());
        stream.close();
        delete [] item->data();
    }
    return true;
}

bool actionList()
{
    if (!checkDatFile()) return false;
    for (std::vector<DatFileItem*>::iterator it = datFile->items()->begin(); it != datFile->items()->end(); ++it)
    {
        DatFileItem* item = *it;
        std::cout << item->name() << std::endl;
    }
    return true;
}

bool actionFormat()
{
    if (!checkDatFile()) return false;
    std::cout << "dat" << datFile->version() << std::endl;
    return true;
}

bool actionVersion()
{
    std::cout << "0.0.1" << std::endl;
    return true;
}

void actionHelp()
{
    std::cout << "Usage: datfile [arguments]" << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  --list, -l list  DAT file items" << std::endl;
    std::cout << "  --version, -v program version" << std::endl;
    std::cout << "  --format, -f DAT file format" << std::endl;
    std::cout << "  --quiet, -q quite mode. Do not display anything" << std::endl;
    std::cout << "  --unpack, -u unpack DAT file" << std::endl;
    std::cout << "  --pack, -p pack pack DAT file" << std::endl;
    std::cout << "  --source, -s path to DAT file" << std::endl;
    std::cout << "  --destination, -d path to extract files" << std::endl;
}

int main(int argc, char** argv)
{    
    for (int i = 0; i != argc; i++)
    {
        std::string argument = argv[i];

        if (argument == "--unpack" || argument == "-u")
        {
            options.action = "unpack";
        }
        else if (argument == "--pack" || argument == "-p")
        {
            options.action = "pack";
        }
        else if (argument == "--list" || argument == "-l")
        {
            options.action = "list";
        }
        else if (argument == "--version" || argument == "-v")
        {
            options.action = "version";
        }
        else if (argument == "--format" || argument == "-f")
        {
            if (i < argc - 1)
            {
                std::string format = argv[i+1];
                if (format == "dat1")
                {
                    i++;
                    options.format = Format::FALLOUT1;
                }
                else if(format == "dat2")
                {
                    i++;
                    options.format = Format::FALLOUT2;
                }
                else
                {
                    options.format = Format::UNKNOWN;
                    options.action = "format";
                }
            }
            else
            {
                options.action = "format";
            }
        }
        else if (argument == "--quiet" || argument == "-q")
        {
            options.quietMode = true;
        }
        else if((argument == "--source" || argument == "-s") && (i < argc-1))
        {
            options.source = argv[++i];
        }
        else if((argument == "--destination" || argument == "-d") && (i < argc-1))
        {
            options.destination = argv[++i];
        }
        else
        {
            options.action = "help";
        }
    }

    bool result = true;
    if (options.action == "unpack") result = actionUnpack();
    if (options.action == "pack") result = actionPack();
    if (options.action == "list") result = actionList();
    if (options.action == "version") result = actionVersion();
    if (options.action == "format") result = actionFormat();

    if (!result && options.quietMode) return 1;

    if (result == false || options.action == "help")
    {
        if (!result)
        {
            std::cout << "[ERROR] " << options.error << std::endl;
        }
        actionHelp();
        return 1;
    }
    return 0;
}
