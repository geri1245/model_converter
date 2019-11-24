# Entry exercise for Shaper3d

OBJ to Binary STL converter based on the references below:

- [OBJ reference](http://paulbourke.net/dataformats/obj/)
- [STL reference](https://en.wikipedia.org/wiki/STL_(file_format)#Binary_STL)

## Build

### Dependencies

1. C++ compiler with c++17 support
2. CMake 3.5 or newer
3. GNU make
4. OpenGL mathematics library (glm)

To install, use:

```bash
sudo apt-get install cmake make libglm-dev
```

### Building the application

```bash
cd shaper
mkdir build && cd build
cmake ..
make
```

### Running the tests

From the build folder, you can run the tests with the following command: ```./bin/test_model_converter```

### Running the program

The binary can be found in ```build/bin/model_converter```

The dafult functionality can be used by supplying 2 parameters to the program.