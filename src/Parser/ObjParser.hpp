#ifndef OBJ_PARSER_HPP
#define OBJ_PARSER_HPP

#include <charconv>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "ModelParser.hpp"

class ObjParser : public ModelParser {
 public:
  virtual ~ObjParser() {}

 protected:
#ifdef OBJ_PARSER_UNITTEST
  friend class ObjParserTest;
#endif
  virtual std::optional<Model> parse_file(std::ifstream& in) final override;
  bool process_line(std::string&& line, Model& model);

  // Tries to read a number from text and returns an optional (value, characters_processed) pair
  template <class Result>
  std::optional<std::pair<Result, std::size_t>> get_number_from_string(const std::string& text);

  // Tries to read at least min, at most max numbers from text into container
  // Returns the number of arguments successfully read
  template <class Container>
  std::optional<std::size_t> read_numbers(const std::string& text,
                                          std::size_t text_position,
                                          Container& container,
                                          const std::size_t min,
                                          const std::size_t max);
};


std::optional<Model> ObjParser::parse_file(std::ifstream& in) {
  Model model;
  std::string line;

  bool success        = true;
  int lines_processed = 0;
  
  while (std::getline(in, line) && success) {
    success = success && process_line(std::move(line), model);
    line.clear();
    ++lines_processed;
  }

  if(!success) {
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
    // TODO
  } else {
    std::cerr << "Unknown line type: " << line_label << "\n";
    return success = false;
  }

  return success;
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

template <class Result>
std::optional<std::pair<Result, std::size_t>> ObjParser::get_number_from_string(const std::string& text) {
  std::size_t chars_processed = 0;
  Result result               = 0;

  try {
    if constexpr (std::is_floating_point_v<Result>) {
      result = std::stod(text, &chars_processed);
    } else if (std::is_integral_v<Result>) {
      result = std::stoi(text, &chars_processed);
    } else {
      std::cerr << "Please give a number as template parameter of \"get_number_from_string\"\n";
    }
  } catch (const std::exception& ex) {
    return std::nullopt;
  }

  // from_chars is not yet available for floating points with g++ 8.3
  // auto [first_not_matching, parse_result] = std::from_chars(text.data(), text.data() + text.size(), result);

  // if(parse_result == std::errc()) {
  //   return std::nullopt;
  // }

  return std::make_pair(result, chars_processed);
}

#endif