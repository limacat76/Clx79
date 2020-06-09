# Clx79
This computer does not exist.

It's my attempt at creating a remotely convincing microcomputer based on a Z80 processor.

Named after Clarisse from The Castle of Cagliostro (1979).

## Aims

The current aim (v1.0) is to run CP/M with Wordstar and Zork.

## Features

Currently Clx79 emulates a CRT screen.

## Getting Started
This project is being developed directly on Windows 10. I am trying to avoid platform dependent code, but since I am using
Visual Studio 2017 CE and running on Windows 10, and not yet testing it in other environments, some windows-isms might slip.

In order to compile the project, you need:
- a source distribution of SDL in C:\SDL\SDL2-devel-2.0.12-VC\SDL2-2.0.12 
- a binary distribution of Boost (for Boost::Parameters) in C:\SDL\local\boost_1_73_0

If you are opening the project for debug on Windows inside Visual Studio you currently need to have SDL2.dll 
(the version of your build, 64 bits, set as default, or 32 bits if you need to change) in the visual studio directory or to 
have the DLL installed on the system. 

## What are you currently working on?

The next step will be implementing a system to load "roms" as a text file.

## What is your future work?
- Implement the rest of the computer (CPU, keyboard and disk emulation)
- Some day: make the solution less dependent on my system configuration

## Need help?
You can contact me at limacat@gmail.com

09.06.2020