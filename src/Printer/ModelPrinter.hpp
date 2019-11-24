#ifndef PRINTER_MODEL_PRINTER_HPP
#define PRINTER_MODEL_PRINTER_HPP

#include <string>
#include <vector>
#include <optional>
#include <fstream>

#include "../Types/Model.hpp"

class ModelPrinter {
public:
  virtual bool print(const Model& model, const std::string& path) = 0;

  virtual ~ModelPrinter() {}

protected:
  virtual std::optional<std::ofstream> open_file(const std::string& path) const;
};

#endif