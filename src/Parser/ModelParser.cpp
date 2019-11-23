#include <fstream>
#include <iostream>
#include <optional>

#include "../Types/Model.hpp"
#include "ModelParser.hpp"

std::optional<Model> ModelParser::parse(const std::string& path) {
  auto in = open_file(path);
  if (!in) {
    return std::nullopt;
  }

  return parse_file(*in);
}

std::optional<std::ifstream> ModelParser::open_file(const std::string& path) const {
  std::ifstream in{path};

  if (in.fail()) {
    std::cerr << "Could not open file " << path << "\n";
    return std::nullopt;
  }

  return std::optional(std::move(in));
}
