#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iterator>

#include "catch/catch.hpp"

#define OBJ_PARSER_UNITTEST

#include "ObjParser.hpp"

class ObjParserTest {
 public:
  ObjParserTest() : obj_parser{std::make_unique<ObjParser>()} {}

  std::optional<Model> parse_file(std::istream& in) {
    return obj_parser->parse_file(in);
  }

  bool process_line(std::string&& line, Model& model) { return obj_parser->process_line(std::move(line), model); }

  std::optional<std::vector<glm::ivec3>> process_faces(const std::string_view& faces_string) {
    return obj_parser->process_faces(faces_string);
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

  SECTION("Correct_position") {
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

  SECTION("Correct_texture") {
    success = test.process_line("vt     -5.000000      -5.000000       0.000000", model);
    REQUIRE(success);
    REQUIRE(model.texture_coords.size() == 1);
    REQUIRE(model.texture_coords[0] == glm::vec3(-5, -5, 0));
  }

  SECTION("Correct_normal") {
    success = test.process_line("vn      12.000000       1.210000       1.000000", model);
    REQUIRE(success);
    REQUIRE(model.normals.size() == 1);
    REQUIRE(model.normals[0] == glm::vec3(12, 1.21, 1));
  }

  SECTION("Correct_faces") {
    success = test.process_line("f 1/1/1     2/2/2    3/3/3  ", model);
    REQUIRE(success);
    REQUIRE(model.triangular_faces.size() == 1);
    REQUIRE(model.triangular_faces[0] ==
            std::array<glm::ivec3, 3>{glm::ivec3{0, 0, 0}, glm::ivec3{1, 1, 1}, glm::ivec3{2, 2, 2}});
  }

  SECTION("Correct_faces_multiple_faces") {
    success = test.process_line("f 1/1/1 2/2/2 3/3/3 4/4/4 23/8989/78", model);
    REQUIRE(success);
    REQUIRE(model.triangular_faces.size() == 3);
    REQUIRE(model.triangular_faces[0] ==
            std::array<glm::ivec3, 3>{glm::ivec3{0, 0, 0}, glm::ivec3{1, 1, 1}, glm::ivec3{2, 2, 2}});
    REQUIRE(model.triangular_faces[1] ==
            std::array<glm::ivec3, 3>{glm::ivec3{0, 0, 0}, glm::ivec3{2, 2, 2}, glm::ivec3{3, 3, 3}});
    REQUIRE(model.triangular_faces[2] ==
            std::array<glm::ivec3, 3>{glm::ivec3{0, 0, 0}, glm::ivec3{3, 3, 3}, glm::ivec3{22, 8988, 77}});
  }

  // SECTION("faces_negative_numbers") {
  //   model.positions.emplace_back(12, 1, 1, 3);   // position 0
  //   model.positions.emplace_back(1, 11, 1, 32);  // position 1
  //   model.positions.emplace_back(1, 1, 15, 3);   // position 2
  //   model.positions.emplace_back(13, 1, 1, 3);   // position 3
  //   model.positions.emplace_back(1, 12, 1, 3);   // position 4

  //   model.texture_coords.emplace_back(11, 1, 1);  // texture 0
  //   model.texture_coords.emplace_back(1, 1, 15);  // texture 1
  //   model.texture_coords.emplace_back(1, 14, 1);  // texture 2
  //   model.texture_coords.emplace_back(11, 1, 1);  // texture 3

  //   model.normals.emplace_back(2, 22, 2);  // normal 0
  //   model.normals.emplace_back(2, 2, 23);  // normal 1
  //   model.normals.emplace_back(22, 2, 2);  // normal 2
  //   model.normals.emplace_back(2, 21, 2);  // normal 3

  //   success = test.process_line("f 1/1/-1 2/1/2 -1/-2/12", model);
  //   REQUIRE(success);
  //   REQUIRE(model.triangular_faces.size() == 1);
  //   REQUIRE(model.triangular_faces[0] ==
  //           std::array<glm::ivec3, 3>{glm::ivec3{0, 0, model.normals.size() - 2},
  //                                     glm::ivec3{1, 0, 1},
  //                                     glm::ivec3{model.positions.size() - 2, model.texture_coords.size() - 3, 11}});
  // }
}

TEST_CASE("correct_file", "[parse_file]") {
  ObjParserTest test;
  SECTION("Regular_file") {
    std::string line_to_process =
        R"(v 12.0 11.23 32.42
vt 0.23 0.34
vn 1.01 2.12 0.12
v 1.12 1.233 12.76
f 1/1/1 2/2/2 3/4/6
)";

std::istringstream input_stream{line_to_process};

    auto result = test.parse_file(input_stream);
    REQUIRE(result);
    REQUIRE(result->positions.size() == 2);
    REQUIRE(result->texture_coords.size() == 1);
    REQUIRE(result->normals.size() == 1);
    REQUIRE(result->triangular_faces.size() == 1);

    REQUIRE(result->positions[0] == glm::vec4{12.0, 11.23, 32.42, 1.0});
    REQUIRE(result->positions[1] == glm::vec4{1.12, 1.233, 12.76, 1.0});
    REQUIRE(result->texture_coords[0] == glm::vec3{0.23, 0.34, 0.0});
    REQUIRE(result->normals[0] == glm::vec3{1.01, 2.12, 0.12});
    REQUIRE(result->triangular_faces[0] ==
            std::array<glm::ivec3, 3>{glm::ivec3{0, 0, 0}, glm::ivec3{1, 1, 1}, glm::ivec3{2, 3, 5}});
  }
}

TEST_CASE("correct_faces", "[process_faces]") {
  ObjParserTest test;

  SECTION("triplets") {
    const std::string faces = "1/2/3 7/9/3 2/3/9";
    const auto result       = test.process_faces(faces);
    REQUIRE(result);
    REQUIRE(result->size() == 3);
    REQUIRE((*result)[0] == glm::ivec3{1, 2, 3});
    REQUIRE((*result)[1] == glm::ivec3{7, 9, 3});
    REQUIRE((*result)[2] == glm::ivec3{2, 3, 9});
  }

  SECTION("6_elements") {
    const std::string faces = "1/2/3 7/9/3 2/3/9 34/78/98 12/34/78 23/98/45";
    const auto result       = test.process_faces(faces);
    REQUIRE(result);
    REQUIRE(result->size() == 6);
    REQUIRE((*result)[0] == glm::ivec3{1, 2, 3});
    REQUIRE((*result)[1] == glm::ivec3{7, 9, 3});
    REQUIRE((*result)[2] == glm::ivec3{2, 3, 9});
    REQUIRE((*result)[3] == glm::ivec3{34, 78, 98});
    REQUIRE((*result)[4] == glm::ivec3{12, 34, 78});
    REQUIRE((*result)[5] == glm::ivec3{23, 98, 45});
  }

  // SECTION("missing_textures") {
  //   const std::string faces = "1//3 7//3 2//9";
  //   const auto result       = test.process_faces(faces);
  //   REQUIRE(result);
  //   REQUIRE(result->size() == 3);
  //   // TODO find out what to check here for textures
  //   REQUIRE((*result)[0] == glm::ivec3{1, 0, 3});
  //   REQUIRE((*result)[1] == glm::ivec3{7, 0, 3});
  //   REQUIRE((*result)[2] == glm::ivec3{2, 0, 9});
  // }

  // SECTION("missing_normals") {
  //   const std::string faces = "1/2/ 7/4/ 2/34/";
  //   const auto result       = test.process_faces(faces);
  //   REQUIRE(result);
  //   REQUIRE(result->size() == 3);
  //   // TODO find out what to check here for textures
  //   REQUIRE((*result)[0] == glm::ivec3{1, 2, 0});
  //   REQUIRE((*result)[1] == glm::ivec3{7, 4, 0});
  //   REQUIRE((*result)[2] == glm::ivec3{2, 34, 0});
  // }
}

TEST_CASE("incorrect_faces", "[process_faces]") {
  ObjParserTest test;

  SECTION("non-numbers") {
    const std::string faces = "1/2/3 asd7/9/3 2/3/9";
    const auto result       = test.process_faces(faces);
    REQUIRE(!result);
  }

  // SECTION("missing_position") {
  //   const std::string faces = "/2/3 /9/3 /3/9";
  //   const auto result = test.process_faces(faces);
  //   REQUIRE(!result);
  // }

  // SECTION("different_format") {
  //   const std::string faces = "23/2/3 423/9/ 53//9";
  //   const auto result = test.process_faces(faces);
  //   REQUIRE(!result);
  // }
}

TEST_CASE("process_incorect_line", "[process_line]") {
  ObjParserTest test;
  Model model;
  bool success = false;

  SECTION("invalid_line_prefix") {
    success = test.process_line("asd 12.000000       1.210000       1.000000", model);
    REQUIRE(success);
    REQUIRE(model.positions.empty());
    REQUIRE(model.texture_coords.empty());
    REQUIRE(model.normals.empty());
    REQUIRE(model.triangular_faces.empty());
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
