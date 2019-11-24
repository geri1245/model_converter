#include <memory>

#include "ModelConverter.hpp"

ModelConverter::ModelConverter(std::unique_ptr<ModelParser> parser, std::unique_ptr<ModelPrinter> printer)
    : parser{std::move(parser)}, printer{std::move(printer)} {}

void ModelConverter::set_parser(std::unique_ptr<ModelParser> parser) { this->parser = std::move(parser); }

void ModelConverter::set_printer(std::unique_ptr<ModelPrinter> printer) { this->printer = std::move(printer); }

bool ModelConverter::parse(const std::string& path) {
  if (!parser) {
    std::cerr << "Please set a parser before trying to parse a file!\n";
    return false;
  }

  auto result = parser->parse(path);
  if (!result) {
    std::cerr << "Failed to parse file: " << path << "\n";
    return false;
  }

  model = std::make_unique<Model>(std::move(*result));

  return true;
}

bool ModelConverter::print(const std::string& path) {
  if (!printer) {
    std::cerr << "Please set a printer before trying to print a file!\n";
    return false;
  }

  if (!model) {
    std::cerr << "Trying to print a model before parsing one!\n";
    return false;
  }

  bool success = printer->print(*model, path);

  return success;
}

const Model* ModelConverter::get_model() const { return model.get(); }