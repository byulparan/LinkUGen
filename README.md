# LinkUGen
Ableton Link compatiable UGen for SuperCollider.  

live coding demo: <https://youtu.be/A_4OC4C7Ptk>  

### download pre-build binary

If your machine is OSX or Linux(i386), then you could download this library.

- [download](https://dl.dropboxusercontent.com/u/23310809/LinkUGen.zip)

### Build Require:

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
    $ make	
```

after build, you should be move "LinkUGen(include Classes, Link.scx)" to SuperCollider Extension direcotry.
