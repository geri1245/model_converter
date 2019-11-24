#include "catch/catch.hpp"

#include "StringMethods.hpp"


TEST_CASE("empty_text_split", "[split_at]") {
  std::string text = "";

  REQUIRE(std::vector<std::string>{} == split_at(text, ','));
}

TEST_CASE("regular_separator", "[split_at]") {
  std::string text = "comma,separated,text";

  const std::vector<std::string> expected = {"comma", "separated", "text"};

  REQUIRE(expected == split_at(text, ','));
}

TEST_CASE("new_line_separator", "[split_at]") {
  std::string text = R"(newline
separated
string)";

  const std::vector<std::string> expected = {"newline", "separated", "string"};

  REQUIRE(expected == split_at(text, '\n'));
}

TEST_CASE("separator_not_in_text", "[split_at]") {
  std::string text = "asdasfsafasdasf";

  const std::vector<std::string> expected = {std::string{text}};

  REQUIRE(expected == split_at(text, ','));
}

TEST_CASE("separators_next_to_each_other", "[split_at]") {
  std::string text = "a///b";

  const std::vector<std::string> expected = {"a", "", "", "b"};

  REQUIRE(expected == split_at(text, '/'));
}

TEST_CASE("separator_at_the_end", "[split_at]") {
  std::string text = "4.23/1.232/";

  const std::vector<std::string> expected = {"4.23", "1.232", ""};

  REQUIRE(expected == split_at(text, '/'));
}

TEST_CASE("separator_at_the_sides", "[split_at]") {
  std::string text = "/4.23/1.232/";

  const std::vector<std::string> expected = {"", "4.23", "1.232", ""};

  REQUIRE(expected == split_at(text, '/'));
}

TEST_CASE("only_separators", "[split_at_trim_separators]") {
  std::string text = "                ";

  REQUIRE(std::vector<std::string>{} == split_at_trim_separators(text, ' '));
}

TEST_CASE("multiple_separators_between_values", "[split_at_trim_separators]") {
  std::string text = "   23        42      442      ";

  REQUIRE(std::vector<std::string>{"23", "42", "442"} == split_at_trim_separators(text, ' '));
}

TEST_CASE("separator_not_present", "[split_at_trim_separators]") {
  std::string text = "   23        42      442      ";

  REQUIRE(std::vector<std::string>{"   23        42      442      "} == split_at_trim_separators(text, ','));
}

TEST_CASE("only_one_separator_at_a_time", "[split_at_trim_separators]") {
  std::string text = "12 34 45 76";

  REQUIRE(std::vector<std::string>{"12", "34", "45", "76"} == split_at_trim_separators(text, ' '));
}

TEST_CASE("real_example", "[split_at_trim_separators]") {
  std::string text = "    1/2/8       2/4/5    ";

  REQUIRE(std::vector<std::string>{"1/2/8", "2/4/5"} == split_at_trim_separators(text, ' '));
}

TEST_CASE("successful_get_number_from_string", "[get_number_from_string]") {
  SECTION("regular_number_with_whitespaces") {
    const std::string str_to_process{"    3.1415"};
    auto result = get_number_from_string<double>(str_to_process);

    REQUIRE(result);
    REQUIRE(result->first == 3.1415);
    REQUIRE(result->second == str_to_process.size());
  }

  SECTION("negative_number") {
    const std::string str_to_process{"-2.45"};
    auto result = get_number_from_string<double>(str_to_process);

    REQUIRE(result);
    REQUIRE(result->first == -2.45);
    REQUIRE(result->second == str_to_process.size());
  }

  SECTION("integer") {
    const std::string str_to_process{"145784"};
    auto result = get_number_from_string<int>(str_to_process);

    REQUIRE(result);
    REQUIRE(result->first == std::stoi(str_to_process));
    REQUIRE(result->second == str_to_process.size());
  }
}

TEST_CASE("unsuccessful_get_number_from_string", "[get_number_from_string]") {
  SECTION("character_with_whitespaces") {
    const std::string str_to_process{"    a.1415"};
    auto result = get_number_from_string<double>(str_to_process);

    REQUIRE(!result);
  }

  SECTION("character_without_whitespaces") {
    const std::string str_to_process{"-aasd"};
    auto result = get_number_from_string<double>(str_to_process);

    REQUIRE(!result);
  }
}