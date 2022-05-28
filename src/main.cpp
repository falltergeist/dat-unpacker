// Project includes
#include "ArgumentsChecker.h"
#include "DatFileUnpacker.h"

// Third party includes
#include <boost/program_options.hpp>

// stdlib
#include <iostream>

using namespace DatUnpacker;

void usage(const boost::program_options::options_description& description)
{
    std::cout << "Unpacker for Fallout 1/2 DAT files" << std::endl;
    std::cout << "v0.0.5 (c) 2012-2022 Falltergeist Developers" << std::endl;
    std::cout << "Example: dat-unpacker -f dat1 -s ~/fallout1/master.dat -d ~/unpacked" << std::endl;
    std::cout << description << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    namespace po = boost::program_options;

    po::options_description argumentsDescription("Arguments");
    argumentsDescription.add_options()
        ("help", "Produce help message")
        ("format,f", po::value<std::string>()->required()->default_value("dat2"), "Fallout DAT file format version. 'dat1' or 'dat2'. 'dat2' is default")
        ("quiet,q", po::bool_switch()->default_value(false), "Quite mode. Do not display anything")
        ("transform,t", po::bool_switch()->default_value(false), "Transform file names to lowercase")
        ("source,s", po::value<std::string>()->required(), "Path to the DAT file")
        ("destination,d", po::value<std::string>()->required(), "Where to extract files");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(argumentsDescription).run(), vm);

    if (vm.count("help")) {
      usage(argumentsDescription);
      return 1;
    }

    try {
      po::notify(vm);
    } catch (std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      usage(argumentsDescription);
      return 1;
    }

    Arguments arguments({
        .quietMode = vm["quiet"].as<bool>(),
        .transformNames = vm["transform"].as<bool>(),
        .source = vm["source"].as<std::string>(),
        .destination = vm["destination"].as<std::string>(),
        .format = vm["format"].as<std::string>() == "dat2" ? Format::FALLOUT2 : Format::FALLOUT1
    });

    ArgumentsChecker argumentsChecker;
    if (!argumentsChecker.check(arguments)) {
        if (!arguments.quietMode) {
            std::cerr << argumentsChecker.getErrorMessage() << std::endl << std::endl;
        }
        return 1;
    }

    DatFileUnpacker datFileUnpacker;
    if (!datFileUnpacker.unpack(arguments)) {
        if (!arguments.quietMode) {
            std::cerr << datFileUnpacker.getErrorMessage() << std::endl << std::endl;
        }
        return 1;
    }

    return 0;
}
