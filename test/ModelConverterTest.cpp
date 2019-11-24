#include "catch/catch.hpp"

#include "ModelConverter.hpp"
#include "ObjParser.hpp"
#include "STLPrinter.hpp"

TEST_CASE("parser", "[ModelConverter]") {
  ModelConverter converter{std::make_unique<ObjParser>(), std::make_unique<STLPrinter>()};

  SECTION("non_existent_file") {
    REQUIRE(!converter.parse("asdasfagsdgsdfgmisonguaiosffnggsdunggsandgaiksundgisandgdsa"));
  }

  SECTION("no_parser") {
    converter.set_parser(nullptr);
    REQUIRE(!converter.parse("asdasf"));
  }
}

TEST_CASE("printer", "[ModelConverter]") {
  ModelConverter converter{std::make_unique<ObjParser>(), std::make_unique<STLPrinter>()};

  SECTION("no_model") {
    REQUIRE(!converter.get_model());
    REQUIRE(!converter.print("out.stl"));
  }

  SECTION("non_existent_directory") {
    REQUIRE(!converter.print("asda/sfag/sdgsdfgmi/songuaio/sffnggsdu/nggsandgai/ksundgi/sandgdsa.stl"));
  }

  SECTION("no_parser") {
    converter.set_printer(nullptr);
    REQUIRE(!converter.print("a.stl"));
  }
}