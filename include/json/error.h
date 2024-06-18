#pragma once

#include <glaze/glaze.hpp>

namespace miningbots::json {

enum Error {
  kSuccess,
  kReadJsonError,
  kCannotStartGameAlreadyStarted,
  kCannotStartGameAlreadyEnded,
  kCannotJoinGameAlreadyJoined,
  kCannotJoinGameInvalidGameId,
  kCannotJoinGameInvalidPlayerKey,
  kCannotSubscribeGameInvalidGameId,
  kCannotSubscribeGameInvalidPlayerId,
  kCannotSubscribeGameInvalidPlayerKey,
  kCannotStartNewJobCurrentJobStillRunning,
  kCannotCancelJobNoJobRunning,
  kUnknownError,
  kNumberOfErrors
};

}
// template <> struct glz::meta<miningbots::json::Error> {
//   using enum miningbots::json::ErrorCode;
//   static constexpr auto value =
//       enumerate(kSuccess, kCannotStartGameAlreadyStarted,
//                 kCannotStartGameAlreadyEnded, kCannotJoinGameAlreadyJoined,
//                 kCannotJoinGameInvalidGameId, kUnknownError,
//                 kNumberOfErrors);
// };