#pragma once

#include <chrono>
#include <cstdint>

using Second = int16_t;
using MilliSecond = uint32_t;
using Microsecond = uint64_t;
using TimeStamp_t = Microsecond;

using Id = uint32_t;
using ResourceId = uint16_t;
using TerrainId = uint16_t;

using PlayerId = Id;
using GameId = Id;
using BotId = Id;
using JobId = Id;
using ActionId = uint16_t;
using VariantId = uint8_t;
using ConfigId = uint16_t;

using Key_t = Id;
using Rarity_t = float;
using Energy_t = uint16_t;
using Kilometer = uint16_t;
using Kilogram = uint16_t;

const std::string kConfigPath = "../config/";
const size_t kMinPlayersToStartGame = 2;
const size_t kMaxPlayers = 4;

const Energy_t kMaxEnergy = 1000;
const Microsecond kEnergyRechargeInterval = 1024768;
const Energy_t kEnergyRechargedPerInterval = 50;

// const MilliSecond kTickInterval = 784;
// const MilliSecond kTickInterval = 1024;
const MilliSecond kTickInterval = 32;

[[maybe_unused]]
static inline TimeStamp_t TimeNow() {
  return std::chrono::duration_cast<std::chrono::microseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

[[maybe_unused]]
static inline std::chrono::microseconds DurationFromMicros(Microsecond micros) {
  return std::chrono::microseconds(micros);
}

template <typename T>
[[maybe_unused]]
static inline constexpr size_t Index(T id) {
  return static_cast<size_t>(id);
}
