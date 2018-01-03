dat-unpacker [![Build Status](https://travis-ci.org/falltergeist/dat-unpacker.svg?branch=develop)](https://travis-ci.org/falltergeist/dat-unpacker)
===

Console utility to unpack Fallout or Fallout 2 game data files

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

- CMake (>= 2.8)
- zlib1g-dev (>= 1.2)

Build:

```bash
mkdir build && cd build && cmake .. && make
```

Usage
===
```
Usage: dat-unpacker [arguments]

Arguments:
    --list, -l list  DAT file items
    --version, -v program version
    --format, -f DAT file format
    --quiet, -q quite mode. Do not display anything
    --unpack, -u unpack DAT file
    --source, -s path to DAT file
    --destination, -d path to extract files
