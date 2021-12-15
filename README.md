# CDP SDK


## Prerequisites
Install the following tools, if you don't already have them:
   * cmake


## Recommended development environment
I strongly encourage the use of CLion. It is created by JetBrains, and all of their stuff is ubiquitous, awesome, industry standard, and is pretty.

It also makes it really easy to build with CMake, not use (often long) terminal commands, and easily run unit tests


## How to build it
After pulling down this repository, open a terminal at this repo's root directory, then execute:

```bash
mkdir build
cd build
cmake ..
make
cd ..
```

If the build was successful, then there will be a `.a` file (specifically, libCDPSDK.a) in `./lib/`. This static library file can be used in any C or C++ project. This includes arduino projects. Read more about static libraries [here](https://medium.com/@meghamohan/all-about-static-libraries-in-c-cea57990c495)