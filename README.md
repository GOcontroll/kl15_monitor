# kl15_monitor
external kl15 monitor to automatically shut down the controller

To build on any system using zig run
```sh
zig build
```
The binary will be in zig-out/bin/

To build with gcc, proper compiler and set it up, enter the directory and run make.
Make sure your compiler doesn't contain a glibc version greater than 2.31


## Usage

arguments: 0 = moduline iv, 1 = moduline mini, 2 = moduline display.

## Changelog

from V1.1.0
added a build.zig to cross compile from any platform

V1.0.0
put libiio.so.0 in /usr/lib/aarch64-linux-gnu/
