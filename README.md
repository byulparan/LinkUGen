# LinkUGen
Ableton Link compatible UGen for SuperCollider.  

live coding demo: <https://youtu.be/A_4OC4C7Ptk>  

### AuthorList
ParkSungmin <byulparan@icloud.com>  
Benjamin Golinvaux <benjamin@golinvaux.com>  

### Download pre-built binaries

**currently, not provide pre-built version. I will update this, as soon as possible.**

### Build Required:

- [CMake](https://cmake.org)

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

after building, you should be move "LinkUGen(include Classes, 'Link.scx' or 'Link.so')" to the SuperCollider Extension directory.
