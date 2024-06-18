#pragma once

#include "position.h"
#include "terrain.h"
#include "json/resource_chunk.h"

#include <vector>
namespace miningbots::json {

struct LandUpdate {
  Position position;
  Terrain terrain;
  bool is_traversable;
  std::vector<ResourceChunk> resources;
};

} // namespace miningbots::json