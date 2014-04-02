#include <iostream>
#include <fstream>
#include "DatFile.h"
#include "DatFileItem.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>

DatFile* datFile = 0;
bool quietMode = false;
std::string action, source, destination, error;

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
    std::cout << "DAT" << datFile->version() << std::endl;
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
            action = "format";
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
