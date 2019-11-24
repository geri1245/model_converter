#ifndef PARSER_OBJ_PARSER_HPP
#define PARSER_OBJ_PARSER_HPP

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
  // Reads input stream line-by-line and calls process_line on each line
  inline virtual std::optional<Model> parse_file(std::istream& in) final override;

  // Process a line. Update model, if it needs to based on the contents of the line
  inline bool process_line(std::string&& line, Model& model);

  // Processes the faces part of a line containing faces ("f 1 2 3" -> processes "1 2 3")
  inline std::optional<std::vector<glm::ivec3>> process_faces(const std::string_view& faces_string);

  // Checks if the result of process_faces is correct. It's only called by process_faces
  inline bool check_faces_vector_syntax(const std::vector<glm::ivec3> faces_vec);

  // Tries to read at least min, at most max numbers from text into container
  // Returns the number of arguments successfully read
  // Whitespaces are discarded at the beginning of each number, but any extra characters at the end
  // will cause this function to fail
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

  const std::string_view line_label{line.data(), first_whitespace};

  if (line_label == "v") {
    glm::vec4 position;
    // The w parameter is optional, default is 1.0
    position.w      = 1.0;
    std::size_t min = 3;
    std::size_t max = 4;

    auto result = read_numbers(line, first_whitespace, position, min, max);

    if (result) {
      model.positions.push_back(std::move(position));
    } else {
      success = false;
    }
  }

  else if (line_label == "vt") {
    glm::vec3 texture;
    // Y and Z are optional, their default is 0
    texture.y       = 0.0;
    texture.z       = 0.0;
    std::size_t min = 1;
    std::size_t max = 3;

    std::optional<std::size_t> result = read_numbers(line, first_whitespace, texture, min, max);

    if (result) {
      model.texture_coords.push_back(std::move(texture));
    } else {
      success = false;
    }
  }

  else if (line_label == "vn") {
    glm::vec3 normals;
    std::size_t min = 3;
    std::size_t max = 3;

    std::optional<std::size_t> result = read_numbers(line, first_whitespace, normals, min, max);

    if (result) {
      model.normals.push_back(std::move(normals));
    } else {
      success = false;
    }
  }

  else if (line_label == "f") {
    auto result = process_faces(std::string_view{line.data() + first_whitespace, line.size() - first_whitespace});

    if (!result) {
      std::cerr << "Error processing faces\n";
      success = false;
    } else if (result) {
      // Preprocess all indices: if they are negative, transform them based on positions/normals/textures size
      // If they are positive or 0, subtract 1 from each to convert 1-based indices to 0-based
      // If the index was 0 (indicates missing value), then it becomes -1, which is not valid for vector indexing
      // So in the final representation -1 will indicate missing values
      for (auto& vec3 : *result) {
        vec3 -= 1;

        // Positions can't be missing, so an x value of 0 is an error
        // Example line for this error: "f /2/3 /4/5 /8/23"
        if (vec3.x == -1) {
          return false;
        } else if (vec3.x < -1) {
          vec3.x = model.positions.size() + vec3.x + 1;
        }

        // For y and z -1 is a valid value, it means they were missing, so we only check for samller than -1 values
        if (vec3.y < -1) {
          vec3.y = model.texture_coords.size() + vec3.y + 1;
        }
        if (vec3.z < -1) {
          vec3.z = model.normals.size() + vec3.z + 1;
        }
      }

      // Non-triangular faces are handled by using "triangle-fan": inedx 0 is always present
      // This way we can handle arbitrary number of face data on a single line
      for (std::size_t i = 1; i + 1 < result->size(); ++i) {
        model.triangular_faces.push_back({(*result)[0], (*result)[i], (*result)[i + 1]});
      }
    }
  } else if (line_label == "#") {
    // Comment, skipping this line
    success = true;
  } else {
    // As not all line types are supported, if we find something else, don't abort, just skip the line
    success = true;
  }

  return success;
}

std::optional<std::vector<glm::ivec3>> ObjParser::process_faces(const std::string_view& faces_string) {
  std::vector<glm::ivec3> faces_result;
  faces_result.reserve(3);
  std::vector<std::string> faces = split_at_trim_separators(faces_string, ' ');

  for (const auto& face : faces) {
    std::vector<std::string> indices = split_at(face, '/');

    // There were more than 2 slashes for one vertex data
    if (indices.size() > 3) {
      return std::nullopt;
    }

    glm::ivec3 vertex_data(0);

    for (std::size_t i = 0; i < indices.size(); ++i) {
      if (indices[i].empty()) {
        // 0 is not a valid obj face index, so we can indicate missing values with it
        vertex_data[i] = 0;
      } else if (auto result = get_number_from_string<int>(indices[i]); result) {
        vertex_data[i] = result->first;
      } else {
        return std::nullopt;
      }
    }

    faces_result.push_back(std::move(vertex_data));
  }

  if (!check_faces_vector_syntax(faces_result)) {
    return std::nullopt;
  }

  return faces_result;
}

bool ObjParser::check_faces_vector_syntax(const std::vector<glm::ivec3> faces_vec) {
  // The syntax must be the same for all faces in the same line. So for example if there is no normal in the first
  // triplet, there mustn't be normals in the later triplets either

  if (faces_vec.empty()) {
    return false;
  }

  std::array<bool, 3> elements_present;
  elements_present.fill(true);

  // Fill up the reference array based on the first triplet
  for (glm::size_t i = 0; i < faces_vec[0].length(); ++i) {
    elements_present[i] = faces_vec[0][i] != 0;
  }

  // Check all lines: if it should be present, but the index is 0 OR if it shouldn't be present but the index is not
  // zero, then the syntax is incorrect and we return false
  for (const auto& triplet : faces_vec) {
    // Position can't be missing
    if (triplet.x == 0) {
      return false;
    }
    for (glm::size_t i = 1; i < faces_vec[0].length(); ++i) {
      if ((elements_present[i] && triplet[i] == 0) || (!elements_present[i] && triplet[i] != 0)) {
        return false;
      }
    }
  }

  return true;
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