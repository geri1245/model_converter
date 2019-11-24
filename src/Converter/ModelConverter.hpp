#ifndef CONVERTER_MODEL_CONVERTER_HPP
#define CONVERTER_MODEL_CONVERTER_HPP

#include <memory>

#include "../Parser/ModelParser.hpp"
#include "../Printer/ModelPrinter.hpp"
#include "../Types/Model.hpp"

// Converter class that has a parser and a printer, both can be changed freely.
class ModelConverter {
 public:
  ModelConverter(std::unique_ptr<ModelParser> parser, std::unique_ptr<ModelPrinter> printer);

  void set_parser(std::unique_ptr<ModelParser> parser);
  void set_printer(std::unique_ptr<ModelPrinter> printer);

  bool parse(const std::string& path);
  bool print(const std::string& path);

  const Model* get_model() const;

 private:
  std::unique_ptr<ModelParser> parser   = nullptr;
  std::unique_ptr<ModelPrinter> printer = nullptr;

  std::unique_ptr<Model> model = nullptr;
};

#endif