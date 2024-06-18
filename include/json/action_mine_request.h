#pragma once

#include "position.h"

namespace miningbots::json {

struct ActionMineRequest {
  GameId game_id;
  PlayerId player_id;
  BotId bot_id;
  Position destination;
  Id target_id; // this is the resource id you want to mine
};

} // namespace miningbots::json
