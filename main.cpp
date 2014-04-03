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

// Falltergeist includes
#include "DatFile.h"
#include "DatFileItem.h"

// Third party includes

DatFile* datFile = 0;
bool quietMode = false;
std::string action, source, destination, error, format;
std::vector<std::string> directories;

void scanDirectory(std::string path)
{
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
                directories.push_back(newpath.substr(source.length() + 1));
                scanDirectory(newpath);
            }
        }
        closedir (dir);
    }
    else
    {
        std::cout << "could not open directory" << std::endl;
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

    std::cout << "test: " << format << std::endl;

    datFile = new DatFile(destination, true);
    datFile->setVersion(format == "dat1" ? 1 : 2);
    datFile->setItems(new std::vector<DatFileItem*>);

    directories.push_back(".");
    scanDirectory(source);

    unsigned int aa = 33;
    *datFile << aa << "test";

    if (format == "dat1")
    {
        for (std::vector<std::string>::iterator it = directories.begin(); it != directories.end(); ++it)
        {
            std::cout << *it << std::endl;
        }


    }


    /*
    if (format == dat1)
    {
        //list directories

        //list directories items
    }
    else
    {
        // list items
    }
    */

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
