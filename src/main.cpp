#include <iostream>
#include <memory>

#include "Model.hpp"
#include "ModelParser.hpp"
#include "ObjParser.hpp"
#include "STLPrinter.hpp"
#include "ModelConverter.hpp"

namespace {
const std::string usage_text = R"(
    OBJ -> binary STL Converter

    The program requires 1 argument, the path to an obj file.
    There is an optional second argument: the name of the output STL file (if no second parameter is
    supplied, it is saved to the current working directory (./out.stl))

    Example: ./model_converter ./cube.obj ../cube.stl

    This will read cube.obj and convert it to cube.stl, placing it into the parent directory
)";
}

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "Please provide an obj file to convert!\n";
    std::cerr << usage_text;
    return -1;
  }

  const std::string file_path   = argv[1];
  const std::string result_path = argc < 3 ? "./out.stl" : argv[2];

  ModelConverter converter{std::make_unique<ObjParser>(), std::make_unique<STLPrinter>()};
  converter.parse(file_path);
  converter.print(result_path);
}