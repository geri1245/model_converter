#include "Model.hpp"

Model::Model(int approximate_size) {
  positions.reserve(approximate_size);
  texture_coords.reserve(approximate_size);
  normals.reserve(approximate_size);
  triangular_faces .reserve(approximate_size);
}