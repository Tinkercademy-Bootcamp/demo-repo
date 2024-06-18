#pragma once

#include "../base_types.h"
#include <cstdint>
#include <string>

namespace miningbots::json {

struct ResourceConfig {
  std::string name{""};
  Rarity_t rarity{0.0};
  Energy_t mining_energy{1};
  MilliSecond mining_time{100};
  uint16_t max_chunk{10}; // the max amount that can be mined with this action
};

} // namespace miningbots::json