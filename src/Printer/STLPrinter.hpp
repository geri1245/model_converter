#ifndef PRINTER_STL_PRINTER_HPP
#define PRINTER_STL_PRINTER_HPP

#include <string>
#include <vector>
#include <optional>
#include <fstream>

#include "ModelPrinter.hpp"
#include "../Types/Model.hpp"

class STLPrinter : public ModelPrinter {
public:
  virtual bool print(const Model& model, const std::string& path) override final;

  virtual ~STLPrinter() {}
};

#endif