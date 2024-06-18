#pragma once

#include "position.h"
#include "resource_chunk.h"

namespace miningbots::json {

struct ActionTransferRequest {
  GameId game_id;
  PlayerId player_id;
  BotId bot_id;
  Position target_position;
  Id target_id; // this is the bot id you are transferring to
  ResourceChunk resource_to_transfer;
};

} // namespace miningbots::json
