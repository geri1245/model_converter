#ifndef PARSER_OBJ_PARSER_HPP
#define PARSER_OBJ_PARSER_HPP

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "../Types/Model.hpp"
#include "ModelParser.hpp"
#include "StringMethods.hpp"

#include <glm/glm.hpp>

class ObjParser : public ModelParser {
 public:
  virtual ~ObjParser() {}

 protected:
#ifdef OBJ_PARSER_UNITTEST
  friend class ObjParserTest;
#endif
  // Reads input stream line-by-line and calls process_line on each line
  virtual std::optional<Model> parse_file(std::istream& in) final override;

  // Process a line. Update model, if it needs to based on the contents of the line
  bool process_line(std::string&& line, Model& model);

  // Processes the faces part of a line containing faces ("f 1 2 3" -> processes "1 2 3")
  std::optional<std::vector<glm::ivec3>> process_faces(const std::string_view& faces_string);

  // Checks if the result of process_faces is correct. It's only called by process_faces
  bool check_faces_vector_syntax(const std::vector<glm::ivec3> faces_vec);
};

#endif