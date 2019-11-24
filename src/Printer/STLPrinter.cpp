#include <fstream>
#include <iostream>
#include <optional>

#include "../Types/Model.hpp"
#include "STLPrinter.hpp"

namespace {
std::ostream& operator<<(std::ostream& out, const glm::vec3& vec) {
  out.write(reinterpret_cast<const char*>(&vec.x), sizeof(float));
  out.write(reinterpret_cast<const char*>(&vec.y), sizeof(float));
  out.write(reinterpret_cast<const char*>(&vec.z), sizeof(float));

  return out;
}

// Same as vec3, we just ignore the fourth parameter
std::ostream& operator<<(std::ostream& out, const glm::vec4& vec) {
  out.write(reinterpret_cast<const char*>(&vec.x), sizeof(float));
  out.write(reinterpret_cast<const char*>(&vec.y), sizeof(float));
  out.write(reinterpret_cast<const char*>(&vec.z), sizeof(float));

  return out;
}
}  // namespace

bool STLPrinter::print(const Model& model, const std::string& path) {
  auto open_result = open_file(path);
  if (!open_result) {
    return false;
  }

  std::ofstream& out = *open_result;

  std::array<char, 80> header;
  header.fill(' ');
  out.write(reinterpret_cast<const char*>(header.data()), header.size());

  uint32_t num_of_faces = model.triangular_faces.size();

  out.write(reinterpret_cast<const char*>(&num_of_faces), sizeof(uint32_t));

  for (const auto& face : model.triangular_faces) {
    // Take the normal vector of the first vertex of the triangle
    out << model.normals[face[0].z];
    out << model.positions[face[0].x] << model.positions[face[1].x] << model.positions[face[2].x];

    uint16_t attrib_byte_cnt = 0;
    out.write(reinterpret_cast<const char*>(&attrib_byte_cnt), sizeof(uint16_t));
  }

  return true;
}
