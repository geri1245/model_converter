#ifndef COMPUTATIONS_COMPUTATIONS_HPP
#define COMPUTATIONS_COMPUTATIONS_HPP

#include <array>

#include <glm/glm.hpp>

#include "../Types/Model.hpp"

// Computes if a ray intersects with a triangle
bool RayIntersectsTriangle(const glm::vec3& rayOrigin,
                           const glm::vec3& rayVector,
                           const std::array<glm::vec3, 3>& inTriangle,
                           glm::vec3& outIntersectionPoint);

// Computes how many times a ray intersects a model
int num_of_intersections(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const Model& model);

// Computes whether a point is inside a model or not based on the number of intersections
bool is_point_inside_model(const glm::vec3& point, const Model& model);

// Calculates the area of a triangle represented as an array of vec3s
float area_of_triangle(const std::array<glm::vec3, 3> & triangle);

// Calculate the surface of a model
float surface_area(const Model& model);

void transform(Model& model, const glm::mat4& transformation);

#endif