# lenia

lenia is an extension of conway's game of life, replacing its binary states and basic interactions with continuous states through space and time, that can interact in infinitely complex ways


## installation and running

you must build this project yourself for now. the only libraries that you should need to install on top of build-essential are libsdl2-dev and libopencv-dev. on an ubuntu-based system:
- ```sudo apt install libsdl2-dev libopencv-dev```
- run ```pkg-config --cflags --libs --static opencv4```, and make sure that the first three flags in the list are equal to the flags in the Makefile row beginning "INCLUDEFLAGS = " (--static only if you will be static building, which is the default, otherwise without that flag and check only the first 2 flags in the output)
- run ```make build -j<n>``` or ```make test -j<n>```, replacing n with the number of threads you want the  build process to use. (it's pretty fast). test just runs the build once it's ready.
- the build can be found at bin/lenia

### status

this project is a full implementation of lenia, with 3 world channels, configurable kernels and growth functions. there are some limitations:
- it gets slow really fast as world resolution increases (this is due to the underlying time complexity of image convolution, even using fast fourier transform)
- kernel parameters and patterns have to be written into the source code, and nothing can really be changed at runtime
- the project is statically linked by default, which requires building openCV with the cmake flag BUILD_SHARED_LIBS=OFF
    - the decision to statically link was made because openCV must be linked with the same version the project was compiled with, which means it could not be run on a machine without the exact same openCV version the build host happened to link it with
    - the code can still be trivially compiled to link dynamically. This basically just needs you to remove all link flags except those actually used, and ensure that your system's openCV build has been build with shared libraries. (don't disable BUILD_SHARED_LIBS, keep almost exclusively -lopencv_core, -lopencv_imgproc)
    - this will change in the future to be less finnicky, by containerising the environment in which the project is built, allowing full control over the version of openCV, how openCV is build, and how the project is linked. All without needing to completely mess up your system libraries!
