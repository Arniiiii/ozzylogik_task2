#ifndef TASK1_CONFIG_HPP
#define TASK1_CONFIG_HPP

#include <array>
#include <cstdint>
#include <string>

#include "task2/fmtlog_verbosity_level.hpp"

namespace task2client
{
  struct Config
  {
    constexpr const static task2::E_verbosityLevel default_verbosity_level
        = task2::E_verbosityLevel::INF;
    constexpr const static std::array default_response_file = std::to_array("result.txt");
    constexpr const static std::array default_destination = std::to_array("127.0.0.1");
    constexpr const static uint16_t default_port = 8001;

    std::string response_file = std::string(default_response_file.data());
    std::string destination = std::string(default_destination.data());
    uint16_t port = default_port;
    task2::E_verbosityLevel verbosity_level = default_verbosity_level;

  } __attribute__((aligned(128)));  // clangd's proposal

}  // namespace task2client

#endif /* TASK1_CONFIG_HPP */
