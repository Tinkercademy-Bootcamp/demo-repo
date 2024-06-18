#include "base_types.h"
#include "client/connection.h"
#include "utils.h"
#include "json/action_move_request.h"
#include "json/client_subscribe_request.h"
#include "json/error_response.h"
#include "json/game_status.h"
#include "json/get_games_response.h"
#include "json/join_game_request.h"
#include "json/join_game_response.h"
#include "json/map_config.h"
#include "json/resource_config.h"

#include "spdlog/fmt/bundled/core.h"
#include "utils.h"
#include "json/update.h"
#include <cpr/cpr.h>
#include <glaze/core/context.hpp>
#include <glaze/json/write.hpp>
#include <ixwebsocket/IXWebSocket.h>
#include <random>
#include <spdlog/spdlog.h>
#include <stdexcept>

std::default_random_engine rng{std::random_device{}()};
std::uniform_int_distribution<Id> id_generator{};

std::vector<Id> generate_player_keys(int n) {
  std::vector<Id> player_keys{};
  for (int i = 0; i < n; i++) {
    player_keys.push_back(id_generator(rng));
  }
  return player_keys;
}

void ThrowExceptionIfError(std::string_view response_text) {
  if (response_text.starts_with("{\"err")) {
    auto error_response =
        ReadJson<miningbots::json::ErrorResponse>(response_text);
    throw std::runtime_error(std::string{response_text});
  }
}

int main(int, char **) {
  using namespace miningbots;

  // https://github.com/gabime/spdlog/wiki/3.-Custom-formatting
  spdlog::set_pattern("[%D %H:%M:%S.%F] [%^%l%$] [%s %!:%#] [%oms] [%ius]  %v");
  spdlog::set_level(spdlog::level::debug);
  // spdlog::cfg::load_env_levels();

  std::string json_str{};

  // example how to write json to string
  auto player_keys = generate_player_keys(10);
  json_str = glz::write_json(player_keys);
  fmt::println("{}", json_str);

  // example how to read string from file and then read json from string
  std::string config_path{"../config/"};
  auto str_buffer = ReadFile(config_path + "player_keys.json");
  auto valid_player_keys_ = ReadJson<std::vector<Key_t>>(str_buffer);

  // example how to read json and handle error case
  str_buffer = ReadFile(config_path + "invalid_json_example.json");
  auto [keys, read_error] =
      ReadJsonWithError<std::vector<Key_t>, glz::parse_error>(str_buffer);
  if (read_error) {
    auto error_message =
        fmt::format("Error: {}", glz::format_error(read_error, json_str));
    SPDLOG_ERROR(error_message);
  }

  // more examples of converting to json. Try creating your own struct and see
  // if you can create a struct that cannot be accepted write_json()
  std::vector<miningbots::json::ResourceConfig> resource_configs{
      {.name = "gold"}, {.name = "iron"}};

  json_str = glz::write_json(resource_configs);
  fmt::println("{}", json_str);

  miningbots::json::MapConfig map_config{10, 10};
  json_str = glz::write_json(map_config);
  fmt::println("{}", json_str);

  std::string hostname = "server.bootcamp.tk.sg";
  // std::string hostname = "127.0.0.1";
  // std::string hostname = "localhost";
  int port = 9002;

  ix::OnMessageCallback websocket_message_callback =
      ([hostname, port](const ix::WebSocketMessagePtr &message) {
        if (message->type == ix::WebSocketMessageType::Message) {
          SPDLOG_INFO("       received ws:{}", message->str);
          if (message->str.starts_with("{\"err")) {
            auto error_response =
                ReadJson<miningbots::json::ErrorResponse>(message->str);
          } else if (message->str.starts_with("{\"upd")) {
            auto my_update = ReadJson<miningbots::json::Update>(message->str);
          }

        } else if (message->type == ix::WebSocketMessageType::Fragment) {
          // no op
        } else if (message->type == ix::WebSocketMessageType::Error) {
          auto error_message = fmt::format(
              "Error: {}\nretries: {}\nWait time(ms): {}\nHTTP Status: {}",
              message->errorInfo.reason, message->errorInfo.retries,
              message->errorInfo.wait_time, message->errorInfo.http_status);
          SPDLOG_ERROR(error_message);
        } else if (message->type == ix::WebSocketMessageType::Open) {
          SPDLOG_INFO("Connected {}:{}", hostname, port);
        } else if (message->type == ix::WebSocketMessageType::Close) {
          SPDLOG_INFO("received close");
        } else if (message->type == ix::WebSocketMessageType::Ping) {
          // no op
        } else if (message->type == ix::WebSocketMessageType::Pong) {
          // no op
        }
      });

  auto response =
      cpr::Get(cpr::Url{fmt::format("http://{}:{}/games", hostname, port)});

  SPDLOG_INFO("{}", response.text);
  ThrowExceptionIfError(response.text);

  GameId game_id = 0;
  auto get_games_response = ReadJson<json::GetGamesResponse>(response.text);
  for (auto &g : get_games_response) {
    if (g.game_status == json::GameStatus::kNotStarted ||
        g.game_status == json::GameStatus::kOpen) {
      game_id = g.game_id;
      break;
    }
  }
  if (game_id == 0) {
    SPDLOG_ERROR("No open games found. {}", response.text);
    return 1;
  }
  SPDLOG_INFO("Found game id {}", game_id);

  json::JoinGameRequest join_game_request{game_id, "Sam's Team", 1621215625};
  response =
      cpr::Put(cpr::Url{fmt::format("http://{}:{}/join_game", hostname, port)},
               cpr::Header{{"Content-Type", "application/json"}},
               cpr::Parameters{
                   {"request", glz::write_json(join_game_request)},
               });
  ThrowExceptionIfError(response.text);
  auto player_id_1 = ReadJson<json::JoinGameResponse>(response.text);
  SPDLOG_INFO("          Received player_id: {} for {}", player_id_1,
              join_game_request.player_name);

  json::JoinGameRequest join_game_request2{game_id, "Peter's Team", 3292005809};
  response =
      cpr::Put(cpr::Url{fmt::format("http://{}:{}/join_game", hostname, port)},
               cpr::Header{{"Content-Type", "application/json"}},
               cpr::Parameters{
                   {"request", glz::write_json(join_game_request2)},
               });
  ThrowExceptionIfError(response.text);
  auto player_id_2 = ReadJson<json::JoinGameResponse>(response.text);
  SPDLOG_INFO("          Received player_id: {} for {}", player_id_2,
              join_game_request2.player_name);

  client::Connection my_connection1(hostname, port, "update",
                                    websocket_message_callback);

  client::Connection my_connection2(hostname, port, "update",
                                    websocket_message_callback);

  json::ClientSubscribeRequest client_subscribe_request{game_id, 1621215625,
                                                        player_id_1};
  my_connection1.sendWSText(glz::write_json(client_subscribe_request));

  json::ClientSubscribeRequest client_subscribe_request2{game_id, 3292005809,
                                                         player_id_2};
  my_connection2.sendWSText(glz::write_json(client_subscribe_request2));

  // json::MoveRequest move_request{123, 456, 0, {23, 45}};
  // response =
  //     cpr::Post(cpr::Url{fmt::format("http://{}:{}/move", hostname, port)},
  //               cpr::Header{{"Content-Type", "application/json"}},
  //               cpr::Parameters{{"move_bot",
  //               glz::write_json(move_request)}});
  // auto beautiful = glz::prettify_json(response.text);
  // SPDLOG_INFO("{}", glz::prettify_json(response.text));
  // ... finally ...
  while (true) {
  }
}
