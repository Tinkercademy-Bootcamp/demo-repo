
#pragma once

#include <cpptrace/cpptrace.hpp>
#include <glaze/glaze.hpp>
#include <spdlog/fmt/bundled/core.h>
#include <spdlog/spdlog.h>
#include <utility>

template <typename T, typename BufferT>
[[maybe_unused]]
static T ReadJson(BufferT json_buffer) {
  T result;
  auto json_error = glz::read_json(result, json_buffer);
  if (json_error) {
    auto error_message =
        fmt::format("{}", glz::format_error(json_error, json_buffer));
    SPDLOG_ERROR(error_message);
    cpptrace::generate_trace().print();

    throw std::runtime_error(error_message);
  }
  return result;
}

template <typename T, typename ParseError>
[[maybe_unused]]
static std::pair<T, ParseError>
ReadJsonWithError(std::string_view json_buffer) {
  T result;
  ParseError parse_error = glz::read_json(result, json_buffer);

  return std::make_pair(result, parse_error);
}

[[maybe_unused]]
static std::string ReadFile(std::string file_name) {
  std::ifstream infile(file_name);
  if (infile.fail()) {
    auto error_message = fmt::format("Error reading file: \"{}\" ", file_name);
    SPDLOG_ERROR(error_message);
    throw std::runtime_error(error_message);
  }
  std::ostringstream str_stream;
  str_stream << infile.rdbuf();
  return str_stream.str();
}

// ryml::Tree ReadYaml(std::string file_name);