# lenia

lenia is an extension of conway's game of life, replacing its binary states and basic interactions with continuous states through space and time, that can interact in infinitely complex ways

## status

this project is a full implementation of lenia, with 3 world channels, configurable kernels and growth functions. there are some limitations:
- it gets slow really fast as world resolution increases
- kernels and patterns have to be written into the source code, and nothing can really be changed at runtime
- systems without openCV can't run this at the moment, so it either needs to be distributed with the project, or statically linked (which may be hard)

### installation and running

you must build this project yourself for now. the only libraries that you should need to install are libsdl2-dev and libopencv-dev. on an ubuntu-based system:
- ```sudo apt install libsdl2-dev libopencv-dev```
- run ```pkg-config --cflags --libs opencv4```, and make sure that the first two flags in the list are equal to the flags in the Makefile row beginning "INCLUDEFLAGS = "
- run ```make build -j<n>``` or ```make test -j<n>```, replacing n with the number of threads you want the  build process to use. (it's pretty fast). test just runs the build once it's ready.
- the build can be found at bin/lenia
