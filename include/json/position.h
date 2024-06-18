#pragma once

#include "../base_types.h"
#include <cstdint>

namespace miningbots::json {

struct Position {
  Kilometer x;
  Kilometer y;

  bool operator==(const Position &other) const {
    return (x == other.x) && (y == other.y);
  }
};

} // namespace miningbots::json

template <> struct std::hash<miningbots::json::Position> {
  std::size_t operator()(const miningbots::json::Position &key) const {
    return hash<int32_t>()((key.x << 14) + key.y);
  }
};
