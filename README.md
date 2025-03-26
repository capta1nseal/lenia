# lenia

Lenia is an extension of conway's game of life, replacing its binary states and basic interactions with continuous states through space and time, that can interact in infinitely complex ways.


## installation and running

Builds are now handled with docker compose.
To build, run `./build` (or `./test` to run immediately after build)
The binary will be output to bin/lenia.


### status

this project is a full implementation of lenia, with 3 world channels, configurable kernels and growth functions. there are some limitations:
- it gets slow really fast as world resolution increases (this is due to the underlying time complexity of image convolution, even using fast fourier transform)
- kernel parameters and patterns have to be written into the source code, and nothing can really be changed at runtime
