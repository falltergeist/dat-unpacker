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

// C++ standard includes
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <dirent.h>
#include <map>

// Falltergeist includes
#include "DatFile.h"
#include "DatFileItem.h"

// Third party includes

DatFile* datFile = 0;
bool quietMode = false;
std::string action, source, destination, error, format;
std::map<std::string, std::vector<DatFileItem*>*> filesList;

void scanDirectory(std::string path)
{
    std::string shortPath = path.substr(source.length() + 1);
    if (shortPath != ".") shortPath = shortPath.substr(2);
    filesList.insert(std::make_pair<std::string, std::vector<DatFileItem*>*>(shortPath, new std::vector<DatFileItem*>));

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
    if (source.length() == 0)
    {
        error = "Source file is not defined";
        return false;
    }
    std::ifstream stream;
    stream.open(source.c_str(), std::ios::binary);
    if (!stream.is_open())
    {
        error = "Can't open source file: " + source;
        return false;
    }

    if (!datFile) datFile = new DatFile(source);

    if (!datFile->version())
    {
        error = source + " is not a DAT file";
        return false;
    }
    return true;
}

bool actionPack()
{
    // source - folder
    // destination - file

    datFile = new DatFile(destination, true);
    datFile->setVersion(format == "dat1" ? 1 : 2);
    datFile->setItems(new std::vector<DatFileItem*>);

    scanDirectory(source + "/.");

    if (format == "dat2")
    {
        for (std::map<std::string, std::vector<DatFileItem*>*>::iterator it = filesList.begin(); it != filesList.end(); ++it)
        {
            std::string directory = it->first;
            std::vector<DatFileItem*>* files = it->second;
            for (std::vector<DatFileItem*>::iterator it = files->begin(); it != files->end(); ++it)
            {
                DatFileItem* item = *it;
                std::string path = source + "/" + directory + "/" + item->name();
                std::cout << directory << "/" << item->name() << std::endl;

                item->setDataOffset(datFile->position());
                std::ifstream stream(path.c_str(), std::ios::binary | std::ios::in);
                unsigned char* buffer = new unsigned char[item->unpackedSize()];
                stream.read((char*)buffer, item->unpackedSize());
                datFile->writeBytes(buffer, item->unpackedSize());
                delete [] buffer;
                stream.close();

                std::string newName = directory + "/" + item->name();
                // Replace slashes and transform to lower case
                std::replace(newName.begin(),newName.end(),'\\','/');
                std::transform(newName.begin(),newName.end(),newName.begin(), ::tolower);
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
            unsigned int filenameSize = item->name().length();
            unsigned char compression = 0;

            *datFile << filenameSize << item->name() << compression << item->unpackedSize() << item->packedSize() << item->dataOffset();
        }

        unsigned int dirTreeSize = datFile->position() - dirTreePosition;
        *datFile << dirTreeSize << (unsigned int) (datFile->position() + 8);
    }
    else if (format == "dat1")
    {
        *datFile << (unsigned int) filesList.size() << (unsigned int) 0x5E << (unsigned int) 0x0 << (unsigned int) 0x0;
        unsigned int dataOffset = 16;
        for (std::map<std::string, std::vector<DatFileItem*>*>::iterator it = filesList.begin(); it != filesList.end(); ++it)
        {
            std::string directory = it->first;
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
            *datFile << (unsigned int) items->size() << (unsigned int) 0x5E << (unsigned int) 0x10 << (unsigned int) 0x0;
            for (std::vector<DatFileItem*>::iterator it = items->begin(); it != items->end(); ++it)
            {
                DatFileItem* item = *it;
                *datFile << (unsigned char) item->name().length() << item->name();
                *datFile << (unsigned int) 0x20; // !NO COMPRESSION
                *datFile << dataOffset << item->unpackedSize() << item->packedSize();
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

                std::string path = source + "/" + directory + "/" + item->name();
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
    if (destination.length() == 0)
    {
        error = "Destination directory is not defined";
        return false;
    }
    else
    {
        struct stat info;
        if(stat(destination.c_str(), &info) != 0)
        {
            error = "Destination directory does not exists: " + destination;
            return false;
        }
        if(!info.st_mode & S_IFDIR )
        {
            error = "Destination is not a directory: " + destination;
            return false;
        }
    }
    // extract items
    for (std::vector<DatFileItem*>::iterator it = datFile->items()->begin(); it != datFile->items()->end(); ++it)
    {
        DatFileItem* item = *it;

        // rtrim
        destination.erase(destination.find_last_not_of("/\\")+1);

        std::string basepath = destination + "/";
        std::string fullpath = basepath + item->name();
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

        if (!quietMode) std::cout << fullpath << std::endl;
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
            action = "unpack";
        }
        else if (argument == "--pack" || argument == "-p")
        {
            action = "pack";
        }
        else if (argument == "--list" || argument == "-l")
        {
            action = "list";
        }
        else if (argument == "--version" || argument == "-v")
        {
            action = "version";
        }
        else if (argument == "--format" || argument == "-f")
        {
            if (i < argc - 1)
            {
                format = argv[i+1];
                if (format == "dat1" || format == "dat2")
                {
                    i++;
                }
                else
                {
                    format = "";
                    action = "format";
                }
            }
            else
            {
                action = "format";
            }
        }
        else if (argument == "--quiet" || argument == "-q")
        {
            quietMode = true;
        }
        else if((argument == "--source" || argument == "-s") && (i < argc-1))
        {
            source = argv[++i];
        }
        else if((argument == "--destination" || argument == "-d") && (i < argc-1))
        {
            destination = argv[++i];
        }
        else
        {
            action = "help";
        }
    }

    bool result = true;
    if (action == "unpack") result = actionUnpack();
    if (action == "pack") result = actionPack();
    if (action == "list") result = actionList();
    if (action == "version") result = actionVersion();
    if (action == "format") result = actionFormat();

    if (!result && quietMode) return 1;

    if (result == false || action == "help")
    {
        if (!result)
        {
            std::cout << "[ERROR] " << error << std::endl;
        }
        actionHelp();
        return 1;
    }
    return 0;
}
