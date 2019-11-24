#include "Computations.hpp"

#include <glm/glm.hpp>

// Möller–Trumbore intersection algorithm
// Source: https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
bool RayIntersectsTriangle(const glm::vec3& rayOrigin,
                           const glm::vec3& rayVector,
                           const std::array<glm::vec3, 3>& inTriangle,
                           glm::vec3& outIntersectionPoint) {
  const float EPSILON = 0.0000001;
  glm::vec3 vertex0   = inTriangle[0];
  glm::vec3 vertex1   = inTriangle[1];
  glm::vec3 vertex2   = inTriangle[2];
  glm::vec3 edge1, edge2, h, s, q;
  float a, f, u, v;
  edge1 = vertex1 - vertex0;
  edge2 = vertex2 - vertex0;
  h     = glm::cross(rayVector, edge2);
  a     = glm::dot(edge1, h);
  if (a > -EPSILON && a < EPSILON) return false;  // This ray is parallel to this triangle.
  f = 1.0 / a;
  s = rayOrigin - vertex0;
  u = f * glm::dot(s, h);
  if (u < 0.0 || u > 1.0) return false;
  q = glm::cross(s, edge1);
  v = f * glm::dot(rayVector, q);
  if (v < 0.0 || u + v > 1.0) return false;
  // At this stage we can compute t to find out where the intersection point is on the line.
  float t = f * glm::dot(edge2, q);
  if (t > EPSILON && t < 1 / EPSILON)  // ray intersection
  {
    outIntersectionPoint = rayOrigin + rayVector * t;
    return true;
  } else  // This means that there is a line intersection but not a ray intersection.
    return false;
}

int num_of_intersections(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const Model& model) {
  int num_of_intersections = 0;

  for (const auto& face : model.triangular_faces) {
    std::array<glm::vec3, 3> triangle{
        glm::vec3{model.positions[face[0].x]},
        glm::vec3{model.positions[face[1].x]},
        glm::vec3{model.positions[face[2].x]},
    };
    glm::vec3 intersection_point;
    if (RayIntersectsTriangle(rayOrigin, rayVector, triangle, intersection_point)) {
      ++num_of_intersections;
    }
  }

  return num_of_intersections;
}

bool is_point_inside_model(const glm::vec3& point, const Model& model) {
  // If num of intersections is even, then point is outside, otherwise inside
  return num_of_intersections(point, glm::vec3{1}, model) % 2 != 0;
}

float area_of_triangle(const std::array<glm::vec3, 3> & triangle) {
  const glm::vec3 AB = triangle[1] - triangle[0];
  const glm::vec3 AC = triangle[2] - triangle[0];

  return glm::length(glm::cross(AB, AC)) / 2.0;
}

float surface_area(const Model& model) {
  float area = 0;
  for (const auto& face : model.triangular_faces) {
    std::array<glm::vec3, 3> triangle{
        glm::vec3{model.positions[face[0].x]},
        glm::vec3{model.positions[face[1].x]},
        glm::vec3{model.positions[face[2].x]},
    };
    
    area += area_of_triangle(triangle);
  }

  return area;
}

void transform(Model& model, const glm::mat4& transformation) {
  for(auto& position : model.positions) {
    position = transformation * position;
  }

  glm::mat4 inverse_transpose_transform = glm::inverse(glm::transpose(transformation));

  for(auto& normal : model.normals) {
    normal = glm::vec3(glm::vec4(normal, 1) * inverse_transpose_transform);
  }
}
