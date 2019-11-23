#ifndef PARSER_MODEL_PARSER_HPP
#define PARSER_MODEL_PARSER_HPP

#include <string>
#include <vector>
#include <optional>
#include <fstream>

#include "../Types/Model.hpp"

class ModelParser {
public:
  virtual std::optional<Model> parse(const std::string& path);

  virtual ~ModelParser() {}

protected:
  virtual std::optional<std::ifstream> open_file(const std::string& path) const;
  virtual std::optional<Model> parse_file(std::ifstream& in) = 0;
};

#endif