#ifndef PRINTER_STL_PRINTER_HPP
#define PRINTER_STL_PRINTER_HPP

#include <string>

#include "../Types/Model.hpp"
#include "ModelPrinter.hpp"

class STLPrinter : public ModelPrinter {
 public:
  virtual bool print(const Model& model, const std::string& path) override final;

  virtual ~STLPrinter() {}
};

#endif