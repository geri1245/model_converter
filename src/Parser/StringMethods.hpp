#ifndef PARSER_STRING_METHODS_HPP
#define PARSER_STRING_METHODS_HPP

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

inline std::vector<std::string> split_at(const std::string_view& text, const char separator);
inline std::vector<std::string> split_at_trim_separators(const std::string_view& text, const char separator);

// Tries to read a number from text and returns an optional (value, characters_processed) pair
template <class Result>
std::optional<std::pair<Result, std::size_t>> get_number_from_string(const std::string& text);


inline std::vector<std::string> split_at(const std::string_view& text, const char separator) {
  std::vector<std::string> tokens;

  for (auto from = text.cbegin(), to = from; to != text.cend();) {
    to = std::find(from, text.cend(), separator);
    tokens.push_back(std::string{from, to});

    from = to;

    if (from != text.cend()) {
      ++from;
    }
  }

  return tokens;
}

std::vector<std::string> split_at_trim_separators(const std::string_view& text, const char separator) {
  std::vector<std::string> tokens;

  auto from = std::find_if(
      text.cbegin(), text.cend(), [separator](std::string::const_iterator::value_type ch) { return ch != separator; });
  auto to = from;

  while (to != text.cend()) {
    to = std::find(from, text.cend(), separator);
    tokens.push_back(std::string{from, to});

    // If there are multiple separators next to each other, remove them
    if (to != text.cend()) {
      ++to;
      from = std::find_if(
          to, text.cend(), [separator](std::string::const_iterator::value_type ch) { return ch != separator; });
      to = from;
    }
  }

  return tokens;
}

template <class Result>
std::optional<std::pair<Result, std::size_t>> get_number_from_string(const std::string& text) {
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