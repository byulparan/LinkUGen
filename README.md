# LinkUGen
Ableton Link compatiable UGen for SuperCollider.  

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

You need dependency libraries for build to LinkUGen.  
You should be clone to repository of SuperCollider3 / Link.  

- [SuperCollider](https://github.com/supercollider/supercollider)
- [Link](https://github.com/Ableton/link)

### Build Instruction:

```{.bash}    
    $ cd "/path/to/LinkUGen"
    $ export SC3_SRC="/PATH/TO/SuerColliderSourceDirectory"
    $ export LINK_SRC="/PATH/TO/LinkSourceDirectory"
	$ export CXX="without OSX. set your C++ compiler. I recommend clang++ higher than 3.6. I build fail g++ on Linux"
    $ cmake .
	$ make
```

after build, you should be move "LinkUGen(include Classes, Link.scx)" to SuperCollider Extension direcotry.
