#pragma once

#include "position.h"

namespace miningbots::json {

struct ActionExplodeRequest {
  GameId game_id;
  PlayerId player_id;
  BotId bot_id;
  Position destination;
};

} // namespace miningbots::json
