#ifndef TYPES_MODEL_REPR_HPP
#define TYPES_MODEL_REPR_HPP

#include <vector>
#include <array>

#include <glm/glm.hpp>

struct Model {
  explicit Model(int approximate_size = 50);

  // Position in homogenous coordinates. If no homogeneous coordinates are used, w = 1.0 is the default
  std::vector<glm::vec4> positions;
  // In case of 2D textures, only x and y are used
  std::vector<glm::vec3> texture_coords;
  std::vector<glm::vec3> normals;
  // Vector containing 3-element arrays, each array containing a triangular face data (position, texture, normal)
  std::vector<std::array<glm::ivec3, 3>> triangular_faces;
};

#endif