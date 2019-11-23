#include <memory>

#include "catch/catch.hpp"

#define OBJ_PARSER_UNITTEST

#include "ObjParser.hpp"

class ObjParserTest {
 public:
  ObjParserTest() : obj_parser{std::make_unique<ObjParser>()} {}

  bool process_line(std::string&& line, Model& model) { return obj_parser->process_line(std::move(line), model); }

  template <class Result>
  std::optional<std::pair<Result, std::size_t>> get_number_from_string(const std::string& text) {
    return obj_parser->get_number_from_string<Result>(text);
  }

  template <class Container>
  std::optional<int> read_numbers(const std::string& text,
                   std::size_t text_position,
                   Container& container,
                   const std::size_t min,
                   const std::size_t max) {
    return obj_parser->read_numbers(text, text_position, container, min, max);
  }

 private:
  std::unique_ptr<ObjParser> obj_parser;
};

TEST_CASE("process_line", "[process_line]") {
  ObjParserTest test;
  Model model;
  bool success = false;

  SECTION("Correct position") {
    success = test.process_line("v      -5.000000       5.000000       0.000000", model);
    REQUIRE(success);
    REQUIRE(model.positions.size() == 1);
    REQUIRE(model.positions[0] == glm::vec4(-5, 5, 0, 1));
  }

  SECTION("Correct_positions_4_components") {
    success = test.process_line("v      -5.000000       5.000000       0.000000   2.500000", model);
    REQUIRE(success);
    REQUIRE(model.positions.size() == 1);
    REQUIRE(model.positions[0] == glm::vec4(-5, 5, 0, 2.5));
  }

SECTION("Correct texture") {
  success = test.process_line("vt     -5.000000      -5.000000       0.000000", model);
  REQUIRE(success);
  REQUIRE(model.texture_coords.size() == 1);
  REQUIRE(model.texture_coords[0] == glm::vec3(-5, -5, 0));
}

SECTION("Correct normal") {
  success = test.process_line("vn      12.000000       1.210000       1.000000", model);
  REQUIRE(success);
  REQUIRE(model.normals.size() == 1);
  REQUIRE(model.normals[0] == glm::vec3(12, 1.21, 1));
}

// // TODO: more testing on faces
// SECTION("Correct faces") {
//   success = test.process_line("f 1/1/1 2/2/2 3/3/3 4/4/4", model);
//   REQUIRE(success);
//   REQUIRE(model.triangular_faces.size() == 2);
//   REQUIRE(model.triangular_faces[0] ==
//           std::array<glm::ivec3, 3>{glm::ivec3{1, 1, 1}, glm::ivec3{2, 2, 2}, glm::ivec3{3, 3, 3}});
//   REQUIRE(model.triangular_faces[1] ==
//           std::array<glm::ivec3, 3>{glm::ivec3{3, 3, 3}, glm::ivec3{2, 2, 2}, glm::ivec3{4, 4, 4}});
// }
}

TEST_CASE("process_incorect_line", "[process_line]") {
  ObjParserTest test;
  Model model;
  bool success = false;

  SECTION("invalid_line_prefix") {
    success = test.process_line("asd 12.000000       1.210000       1.000000", model);
    REQUIRE(!success);
  }

  SECTION("fewer_than_three positions") {
    success = test.process_line("v 12.000000 13.000000", model);
    REQUIRE(!success);
  }

  SECTION("Junk_at_the_end_of_line") {
    success = test.process_line("v      -5.000000       5.000000       0.000000   2.500000 asdasdasd", model);
    REQUIRE(!success);
  }

  SECTION("Junk_in_the_middle") {
    success = test.process_line("v      -5.000000       5.000000   asd    0.000000   2.500000", model);
    REQUIRE(!success);
  }
}

TEST_CASE("successful_get_number_from_string", "[get_number_from_string]") {
  ObjParserTest test;

  SECTION("regular_number_with_whitespaces") {
    const std::string str_to_process{"    3.1415"};
    auto result = test.get_number_from_string<double>(str_to_process);

    REQUIRE(result);
    REQUIRE(result->first == 3.1415);
    REQUIRE(result->second == str_to_process.size());
  }

  SECTION("negative_number") {
    const std::string str_to_process{"-2.45"};
    auto result = test.get_number_from_string<double>(str_to_process);

    REQUIRE(result);
    REQUIRE(result->first == -2.45);
    REQUIRE(result->second == str_to_process.size());
  }

  SECTION("integer") {
    const std::string str_to_process{"145784"};
    auto result = test.get_number_from_string<int>(str_to_process);

    REQUIRE(result);
    REQUIRE(result->first == std::stoi(str_to_process));
    REQUIRE(result->second == str_to_process.size());
  }
}

TEST_CASE("unsuccessful_get_number_from_string", "[get_number_from_string]") {
  ObjParserTest test;

  SECTION("character_with_whitespaces") {
    const std::string str_to_process{"    a.1415"};
    auto result = test.get_number_from_string<double>(str_to_process);

    REQUIRE(!result);
  }

  SECTION("character_without_whitespaces") {
    const std::string str_to_process{"-aasd"};
    auto result = test.get_number_from_string<double>(str_to_process);

    REQUIRE(!result);
  }
}

TEST_CASE("read_numbers_successful", "[read_numbers]") {
  ObjParserTest test;
  std::vector<double> container;
  container.resize(3);

  SECTION("multiple_numbers_with whitespaces") {
    const std::string str_to_process{"3.1415    2.23    1.43"};
    auto result = test.read_numbers(str_to_process, 0, container, 3, 3);

    REQUIRE(result);
    REQUIRE(*result == 3);
    REQUIRE(container[0] == 3.1415);
    REQUIRE(container[1] == 2.23);
    REQUIRE(container[2] == 1.43);
  }

  SECTION("upper_part_of_interval") {
    const std::string str_to_process{"3.1415    2.23    -1.43"};
    auto result = test.read_numbers(str_to_process, 0, container, 1, 3);

    REQUIRE(result);
    REQUIRE(*result == 3);
    REQUIRE(container[0] == 3.1415);
    REQUIRE(container[1] == 2.23);
    REQUIRE(container[2] == -1.43);
  }

  SECTION("lower_part_of_interval") {
    const std::string str_to_process{"        -3.1415"};
    auto result = test.read_numbers(str_to_process, 0, container, 1, 3);

    REQUIRE(result);
    REQUIRE(*result == 1);
    REQUIRE(container[0] == -3.1415);
  }

  SECTION("middle_of_interval") {
    const std::string str_to_process{"3.1415    2.23    1.43"};
    auto result = test.read_numbers(str_to_process, 0, container, 2, 4);

    REQUIRE(result);
    REQUIRE(*result == 3);
    REQUIRE(container[0] == 3.1415);
    REQUIRE(container[1] == 2.23);
    REQUIRE(container[2] == 1.43);
  }
}

TEST_CASE("read_numbers_unsuccessful", "[read_numbers]") {
  ObjParserTest test;
  std::vector<double> container;
  container.resize(4);

  SECTION("too_few_numbers_read") {
    const std::string str_to_process{"3.1415"};
    auto result = test.read_numbers(str_to_process, 0, container, 2, 3);

    REQUIRE(!result);
    REQUIRE(container[0] == 3.1415);
  }

  SECTION("non_number_characters") {
    const std::string str_to_process{"3.1415 a"};
    auto result = test.read_numbers(str_to_process, 0, container, 2, 3);

    REQUIRE(!result);
    REQUIRE(container[0] == 3.1415);
  }

  SECTION("too_many_numbers_read") {
    const std::string str_to_process{"3.1415 2 3 4"};
    auto result = test.read_numbers(str_to_process, 0, container, 2, 3);

    REQUIRE(!result);
    REQUIRE(container[0] == 3.1415);
    REQUIRE(container[1] == 2);
    REQUIRE(container[2] == 3);
  }
}
