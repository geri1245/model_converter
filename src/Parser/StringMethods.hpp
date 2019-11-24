#ifndef PARSER_STRING_METHODS_HPP
#define PARSER_STRING_METHODS_HPP

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

// Splits text at separator
inline std::vector<std::string> split_at(const std::string_view& text, const char separator);
// Split text at separator, but duplicated separators and separators at the end/start of text are removed
inline std::vector<std::string> split_at_trim_separators(const std::string_view& text, const char separator);

// Tries to read a number from text and returns an optional (value, characters_processed) pair
template <class Result>
std::optional<std::pair<Result, std::size_t>> get_number_from_string(const std::string& text);

// Tries to read at least min, at most max numbers from text into container
// Returns the number of arguments successfully read
// Whitespaces are discarded at the beginning of each number, but any extra characters at the end
// will cause this function to fail
template <class Container>
std::optional<std::size_t> read_numbers(const std::string& text,
                                        std::size_t text_position,
                                        Container& container,
                                        const std::size_t min,
                                        const std::size_t max);


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

template <class Container>
std::optional<std::size_t> read_numbers(const std::string& text,
                                        std::size_t text_position,
                                        Container& container,
                                        const std::size_t min,
                                        const std::size_t max) {
  std::size_t numbers_read         = 0;
  std::size_t part_to_process_next = text_position;

  // Read the first 3 coordinates
  while (numbers_read < max && part_to_process_next < text.size()) {
    if (part_to_process_next < text.size()) {
      auto result = get_number_from_string<double>(std::string{text, part_to_process_next});
      if (result) {
        container[numbers_read] = result->first;
        part_to_process_next += result->second;
        ++numbers_read;
      } else {
        // Something went wrong while reading numbers
        return std::nullopt;
      }
    } else {
      // We reached the end before we should have, there was an error
      return std::nullopt;
    }
  }

  if (numbers_read < min || numbers_read > max || part_to_process_next != text.size()) {
    return std::nullopt;
  }

  return numbers_read;
}

#endif