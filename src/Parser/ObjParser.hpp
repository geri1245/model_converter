#ifndef OBJ_PARSER_HPP
#define OBJ_PARSER_HPP

#include <charconv>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "ModelParser.hpp"
#include "StringMethods.hpp"

class ObjParser : public ModelParser {
 public:
  virtual ~ObjParser() {}

 protected:
#ifdef OBJ_PARSER_UNITTEST
  friend class ObjParserTest;
#endif
  virtual std::optional<Model> parse_file(std::istream& in) final override;
  bool process_line(std::string&& line, Model& model);
  std::optional<std::vector<glm::ivec3>> process_faces(const std::string_view& faces_string);

  // Tries to read at least min, at most max numbers from text into container
  // Returns the number of arguments successfully read
  template <class Container>
  std::optional<std::size_t> read_numbers(const std::string& text,
                                          std::size_t text_position,
                                          Container& container,
                                          const std::size_t min,
                                          const std::size_t max);
};


std::optional<Model> ObjParser::parse_file(std::istream& in) {
  Model model;
  std::string line;

  bool success        = true;
  int lines_processed = 0;

  while (std::getline(in, line) && success) {
    success = success && process_line(std::move(line), model);
    line.clear();
    ++lines_processed;
  }

  if (!success) {
    std::cerr << "Failed to read line " << lines_processed << "\n";
    return std::nullopt;
  }

  return model;
}

bool ObjParser::process_line(std::string&& line, Model& model) {
  bool success          = true;
  auto first_whitespace = line.find_first_of(' ');

  const std::string_view line_label{line.c_str(), first_whitespace};

  if (line_label == "v") {
    glm::vec4 position;
    position.w      = 1.0;
    std::size_t min = 3;
    std::size_t max = 4;

    std::optional<std::size_t> numbers_read = read_numbers(line, first_whitespace, position, min, max);

    success = numbers_read && *numbers_read >= min && *numbers_read <= max;
    if (success) {
      model.positions.push_back(std::move(position));
    }
  } else if (line_label == "vt") {
    glm::vec3 texture;
    texture.y       = 0.0;
    texture.z       = 0.0;
    std::size_t min = 1;
    std::size_t max = 3;

    std::optional<std::size_t> numbers_read = read_numbers(line, first_whitespace, texture, min, max);

    success = numbers_read && *numbers_read >= min && *numbers_read <= max;
    if (success) {
      model.texture_coords.push_back(std::move(texture));
    }
  } else if (line_label == "vn") {
    glm::vec3 normals;
    std::size_t min = 3;
    std::size_t max = 3;

    std::optional<std::size_t> numbers_read = read_numbers(line, first_whitespace, normals, min, max);

    success = numbers_read && *numbers_read >= min && *numbers_read <= max;
    if (success) {
      model.normals.push_back(std::move(normals));
    }
  } else if (line_label == "f") {
    auto result = process_faces(std::string_view{line.data() + first_whitespace, line.size() - first_whitespace});

    if (!result) {
      std::cerr << "Error processing faces\n";
      success = false;
    } else if (result) {
      for (std::size_t i = 1; i + 1 < result->size(); ++i) {
        // TODO handle negative indices
        // TODO handle if not all components are present

        // The indexing is 1-based in obj, convert it to 0-based
        model.triangular_faces.push_back({(*result)[0] - 1, (*result)[i] - 1, (*result)[i + 1] - 1});
      }
    }

  } else {
    std::cerr << "Unknown line type: " << line_label << "\n";
    success = false;
  }

  return success;
}

std::optional<std::vector<glm::ivec3>> ObjParser::process_faces(const std::string_view& faces_string) {
  std::vector<glm::ivec3> faces_result;
  faces_result.reserve(3);
  std::vector<std::string> faces = split_at_trim_separators(faces_string, ' ');

  // TODO: check for missing textures/normals (no textures)
  for (const auto& face : faces) {
    std::vector<std::string> indices = split_at(face, '/');

    // There were more than 2 slashes for one vertex data
    if (indices.size() > 3) {
      return std::nullopt;
    }

    glm::ivec3 vertex_data;

    for (std::size_t i = 0; i < indices.size(); ++i) {
      if (indices[i].empty()) {
        // TODO handle case of missing data
      }

      if (auto result = get_number_from_string<int>(indices[i]); result) {
        vertex_data[i] = result->first;
      } else {
        return std::nullopt;
      }
    }

    faces_result.push_back(std::move(vertex_data));
  }

  return faces_result;
}

template <class Container>
std::optional<std::size_t> ObjParser::read_numbers(const std::string& text,
                                                   std::size_t text_position,
                                                   Container& container,
                                                   const std::size_t min,
                                                   const std::size_t max) {
  std::size_t numbers_read         = 0;
  std::size_t part_to_process_next = text_position;

  // Read the first 3 coordinates
  while (numbers_read < max && part_to_process_next < text.size()) {
    if (part_to_process_next < text.size()) {
      auto result = get_number_from_string<double>(std::string{text, part_to_process_next});
      if (result) {
        container[numbers_read] = result->first;
        part_to_process_next += result->second;
        ++numbers_read;
      } else {
        // Something went wrong while reading numbers
        return std::nullopt;
      }
    } else {
      // We reached the end before we should have, there was an error
      return std::nullopt;
    }
  }

  if (numbers_read < min || numbers_read > max || part_to_process_next != text.size()) {
    return std::nullopt;
  }

  return numbers_read;
}

#endif