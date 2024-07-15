# OBJ to Binary STL converter for Linux

The file formats are based on the following references:

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
cd model_converter
mkdir build && cd build
cmake ..
make -j8
```

### Running the tests

From the build folder, you can run the tests with the following command: ```./bin/test_model_converter```

### Running the program

The binary can be found in ```build/bin/model_converter```

The dafult functionality can be used by supplying 2 parameters to the program.

1. ```path/to/obj/file``` - this is a mandatory argument
2. ```path/to/output/file``` - this is optional, if not supplied, the output will be ```./out.stl```

**Example**

```bash
./bin/model_converter some/obj/file.obj subfolder/output.stl
```
This will convert ```file.obj``` in folder ```some/obj``` into an stl file and place it into ```subfolder/ouput.stl```.

### Other functionality

There are a few other functions, that can't be used from the command line interface (yet). However they can be used from c++ code and all of them operate on ```Model``` types, that are the inner representation of obj files. You can found them in ```Computations.hpp```. There are also examples of how to use them in the unit tests, namely ```ComputationsTest.cpp```

1. ```transform(model, transformation)``` - transform every vertex position with the given transformation. Transformations can be generated using the built-in glm functions, as well as combined with matrix multiplication:
  - ```glm::translate(vector)```
  - ```glm::scale(vector)```
  - ```glm::rotate(angle, vector)```
  - ```glm::translate(vec1) * glm::rotate(alpha, vec2)``` - creates a transformation matrix that first rotates, then translates.

2. ```surface_area(model)``` - computes the surface area of the model. This is the area of all the triangles the model consits of.
3. ```is_point_inside_model(point, model)``` - checks whether the given point is inside the model or outside. Uses triangle intersection with every triangle of the model. If the number of intersections is even, the point is outside, otherwise inside.
