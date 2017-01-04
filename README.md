# LinkUGen
Ableton Link compatible UGen for SuperCollider.  

live coding demo: <https://youtu.be/A_4OC4C7Ptk>  

### AuthorList
ParkSungmin <byulparan@icloud.com>  
Benjamin Golinvaux <benjamin@golinvaux.com>  

### Download pre-built binaries

It include OSX / Linux(i386) / Windows(x64/x86) pre-built binaries.

- [download](https://dl.dropboxusercontent.com/u/23310809/LinkUGen.zip)

### Build Require:

- [CMake](https://cmake.org)
- [Boost](http://www.boost.org)
- The Asio package (at least in ArchLinux)

LinkUGen has some dependencies.
You need to download (or clone) the SuperCollider3 and Link sources:  

- [SuperCollider](https://github.com/supercollider/supercollider)
- [Link](https://github.com/Ableton/link)

### Build Instruction:

```{.bash}    
    $ cd "/path/to/LinkUGen"
    $ export SC3_SRC="/PATH/TO/SuperColliderSourceDirectory"
    $ export LINK_SRC="/PATH/TO/LinkSourceDirectory"
    $ export CXX="if not on OSX, set your C++ compiler. I recommend clang++ higher than 3.6. I failed to build it with g++ in Linux"
    $ cmake .
    $ make
```

after building, you should be move "LinkUGen(include Classes, Link.scx)" to the SuperCollider Extension directory.
