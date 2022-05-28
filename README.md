dat-unpacker
===
[![Version](https://img.shields.io/github/release/falltergeist/dat-unpacker.svg)](https://github.com/falltergeist/dat-unpacker/releases/latest) [![Discord](https://img.shields.io/discord/401990446747877376.svg)](https://discord.gg/jxs6WRq)

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
- [zlib](https://github.com/madler/zlib)
- [boost::program_options](https://github.com/boostorg/program_options)

Build:

```bash
cmake . && make
```

Usage
===
```
$ dat-unpacker --help                                               
Unpacker for Fallout 1/2 DAT files
v0.0.5 (c) 2012-2022 Falltergeist Developers
Example: dat-unpacker -f dat1 -s ~/fallout1/master.dat -d ~/unpacked
Arguments:
  --help                      Produce help message
  -f [ --format ] arg (=dat2) Fallout DAT file format version. 'dat1' or 
                              'dat2'. 'dat2' is default
  -q [ --quiet ]              Quite mode. Do not display anything
  -t [ --transform ]          Transform file names to lowercase
  -s [ --source ] arg         Path to the DAT file
  -d [ --destination ] arg    Where to extract files
```
