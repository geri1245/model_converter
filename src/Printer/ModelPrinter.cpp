#include <fstream>
#include <iostream>
#include <optional>

#include "../Types/Model.hpp"
#include "ModelPrinter.hpp"

std::optional<std::ofstream> ModelPrinter::open_file(const std::string& path) const {
  std::ofstream out{path, std::ios::binary};

  if (out.fail()) {
    std::cerr << "Could not open file for writing " << path << "\n";
    return std::nullopt;
  }

  return std::optional(std::move(out));
}
