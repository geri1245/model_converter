#include "Model.hpp"

Model::Model(int approximate_size) {
  positions.reserve(approximate_size);
  texture_coords.reserve(approximate_size);
  normals.reserve(approximate_size);
  triangular_faces .reserve(approximate_size);
}

namespace debug {
std::ostream& operator<<(std::ostream& out, const glm::ivec3& vec) {
  out << vec.x << '/' << vec.y << '/' << vec.z;

  return out;
}

std::ostream& operator<<(std::ostream& out, const glm::vec4& vec) {
  out << vec.x << ' ' << vec.y << ' ' << vec.z << ' ' << vec.w << "\n";

  return out;
}

std::ostream& operator<<(std::ostream& out, const glm::vec3& vec) {
  out << vec.x << ' ' << vec.y << ' ' << vec.z << "\n";

  return out;
}

template <class A>
std::ostream& operator<<(std::ostream& out, const std::vector<A>& container) {
  for(const auto& elem : container) {
    out << elem;
  }

  return out;
}

template <class A, std::size_t N>
std::ostream& operator<<(std::ostream& out, const std::array<A, N>& container) {
  for(const auto& elem : container) {
    out << elem << " ";
  }

  out << "\n";

  return out;
}


std::ostream& operator<<(std::ostream& out, const Model& model) {
  std::operator<<(out, "Positions:\n");
  for(const auto & position : model.positions) {
    out << position;
  }

  std::operator<<(out, "Textures:\n");
  for(const auto & texture : model.texture_coords) {
    out << texture;
  }

  std::operator<<(out, "Normals:\n");
  for(const auto & normal : model.normals) {
    out << normal;
  }

  std::operator<<(out, "Faces:\n");
  for(const auto & face : model.triangular_faces) {
    out << face;
  }

  return out;
}

}