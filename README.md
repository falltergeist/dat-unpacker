dat-unpacker [![Build Status](https://travis-ci.org/falltergeist/dat-unpacker.svg?branch=develop)](https://travis-ci.org/falltergeist/dat-unpacker) [![Version](https://img.shields.io/github/release/falltergeist/dat-unpacker.svg)](https://github.com/falltergeist/dat-unpacker/releases/latest) [![Discord](https://img.shields.io/discord/401990446747877376.svg)](https://discord.gg/jxs6WRq)
===

Console utility to unpack Fallout 1 or Fallout 2 DAT files

Installation:
=============

Ubuntu
------

```bash
sudo add-apt-repository ppa:falltergeist/falltergeist
sudo apt-get update
sudo apt-get install dat-unpacker
```

Compilation from source:
============

Dependencies:
-------------

- compiler which supports the C++17 standard and the filesystem library
- CMake (>= 3.10)
- zlib1g-dev (>= 1.2)

Build:

```bash
mkdir build && cd build && cmake .. && make
```

Usage
===
```
Usage: dat-unpacker [arguments]
Example: dat-unpacker -f dat1 -s ~/fallout1/master.dat -d ~/unpacked

Arguments:
  --format, -f        Fallout DAT file format version. 'dat1' or 'dat2'. 'dat2' is default
                      Possible values are: 'dat1', 'dat2'.
                      (Defaults to 'dat2')
  --quiet, -q         Quite mode. Do not display anything
  --transform, -t     Transform file names to lowercase
  --source, -s        Path to the DAT file
  --destination, -d   Where to extract files
```
