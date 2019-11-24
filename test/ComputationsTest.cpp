#include "catch/catch.hpp"

#include "Computations.hpp"

TEST_CASE("intersection", "[RayIntersectsTriangle]") {
  const std::array<glm::vec3, 3> triangle = {glm::vec3{0, 0, 0}, glm::vec3{2, 0, 0}, glm::vec3{0, 2, 0}};
  const glm::vec3 origin{0, 0, 2};
  const glm::vec3 ray_dir{0.4, 0.2, -1};
  glm::vec3 intersection_point;

  bool intersects = RayIntersectsTriangle(origin, ray_dir, triangle, intersection_point);

  REQUIRE(intersects);
  REQUIRE(intersection_point == glm::vec3{0.8, 0.4, 0});
}

TEST_CASE("intersection_3D_point", "[RayIntersectsTriangle]") {
  const std::array<glm::vec3, 3> triangle = {glm::vec3{0, 0, 0}, glm::vec3{20, 3, -4}, glm::vec3{2, 2, -20}};
  const glm::vec3 origin{10, -3, -10};
  const glm::vec3 ray_dir{0, 1, 0};
  glm::vec3 intersection_point;

  bool intersects = RayIntersectsTriangle(origin, ray_dir, triangle, intersection_point);

  REQUIRE(intersects);
}

TEST_CASE("no_intersection_3D_point", "[RayIntersectsTriangle]") {
  const std::array<glm::vec3, 3> triangle = {glm::vec3{0, 0, 0}, glm::vec3{20, 3, -4}, glm::vec3{2, 2, -20}};
  const glm::vec3 origin{10, -3, -10};
  const glm::vec3 ray_dir{1, 0, 0};
  glm::vec3 intersection_point;

  bool intersects = RayIntersectsTriangle(origin, ray_dir, triangle, intersection_point);

  REQUIRE(!intersects);
}

TEST_CASE("no_intersection", "[RayIntersectsTriangle]") {
  const std::array<glm::vec3, 3> triangle = {glm::vec3{0, 0, 0}, glm::vec3{2, 0, 0}, glm::vec3{0, 2, 0}};
  const glm::vec3 origin{0, 0, 2};
  const glm::vec3 ray_dir{0.4, 0.2, 1};
  glm::vec3 intersection_point;

  bool intersects = RayIntersectsTriangle(origin, ray_dir, triangle, intersection_point);

  REQUIRE(!intersects);
}

TEST_CASE("cube_intersections", "[num_of_intersections, is_point_inside_model]") {
  Model model;
  model.positions.emplace_back(0.0, 0.0, 0.0, 1);
  // The first element is a dummy, so we get 1-based indexing
  model.positions.emplace_back(0.0, 0.0, 0.0, 1);
  model.positions.emplace_back(0.0, 0.0, 1.0, 1);
  model.positions.emplace_back(0.0, 1.0, 0.0, 1);
  model.positions.emplace_back(0.0, 1.0, 1.0, 1);
  model.positions.emplace_back(1.0, 0.0, 0.0, 1);
  model.positions.emplace_back(1.0, 0.0, 1.0, 1);
  model.positions.emplace_back(1.0, 1.0, 0.0, 1);
  model.positions.emplace_back(1.0, 1.0, 1.0, 1);

  // We only care about position indices now, these indices are the required positions from
  // "assets/simple_cube_with_normals"
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{1}, glm::ivec3{7}, glm::ivec3{5}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{1}, glm::ivec3{3}, glm::ivec3{7}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{1}, glm::ivec3{4}, glm::ivec3{3}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{1}, glm::ivec3{2}, glm::ivec3{4}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{3}, glm::ivec3{8}, glm::ivec3{7}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{3}, glm::ivec3{4}, glm::ivec3{8}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{5}, glm::ivec3{7}, glm::ivec3{8}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{5}, glm::ivec3{8}, glm::ivec3{6}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{1}, glm::ivec3{5}, glm::ivec3{6}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{1}, glm::ivec3{6}, glm::ivec3{2}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{2}, glm::ivec3{6}, glm::ivec3{8}});
  model.triangular_faces.push_back(std::array<glm::ivec3, 3>{glm::ivec3{2}, glm::ivec3{8}, glm::ivec3{4}});

  SECTION("num_of_intersections_from_middle_of_cube") {
    REQUIRE(num_of_intersections(glm::vec3{0.5, 0.5, 0.5}, glm::vec3{-1.34, 0.4, 1}, model) == 1);
    REQUIRE(num_of_intersections(glm::vec3{0.5, 0.5, 0.5}, glm::vec3{1, -3.6, 12.2}, model) == 1);
    REQUIRE(num_of_intersections(glm::vec3{0.5}, glm::vec3{-3.5, 1.23, 1}, model) == 1);
    REQUIRE(num_of_intersections(glm::vec3{0.5}, glm::vec3{10, -3.54, 12.0}, model) == 1);
  }

  SECTION("num_of_intersections_from_outside_the_cube") {
    REQUIRE(num_of_intersections(glm::vec3{-0.4, 0.1, 0.7}, glm::vec3{1, 0, 0}, model) == 2);
    REQUIRE(num_of_intersections(glm::vec3{-0.4, 0.1, 0.7}, glm::vec3{1, 0.3, 0}, model) == 2);
    REQUIRE(num_of_intersections(glm::vec3{1.4, 0.1, 0.7}, glm::vec3{-5, 0.3, 2}, model) == 2);
  }

  SECTION("is_point_inside") {
    REQUIRE(is_point_inside_model(glm::vec3{0.4, 0.7,  0.2}, model));
    REQUIRE(is_point_inside_model(glm::vec3{0.4, 0.9,  0.32}, model));
    REQUIRE(is_point_inside_model(glm::vec3{0.4, 0.32, 0.7},  model));

    REQUIRE(!is_point_inside_model(glm::vec3{-0.4, 0.32, 0.7},  model));
    REQUIRE(!is_point_inside_model(glm::vec3{0.4, 1.32, 0.7},  model));
    REQUIRE(!is_point_inside_model(glm::vec3{0.4, 0.32, 21.7},  model));
  }

  SECTION("cube_area") {
    REQUIRE(surface_area(model) == 6);
  }
}

TEST_CASE("simple_triangles", "[area_of_triangle]") {
  std::array<glm::vec3, 3> triangle = {glm::vec3{0, 0, 0}, glm::vec3{1, 0, 0}, glm::vec3{0, 3, 0}};
  REQUIRE(area_of_triangle(triangle) == 1.5);

  triangle = std::array<glm::vec3, 3>{glm::vec3{1, 0, 0}, glm::vec3{1, 1, 0}, glm::vec3{1, 1, 2}};
  REQUIRE(area_of_triangle(triangle) == 1.0);
}
