#ifndef TASK1_CONFIG_HPP
#define TASK1_CONFIG_HPP

#include <array>
#include <cstdint>
#include <string>

#include "task2/fmtlog_verbosity_level.hpp"

namespace task2server
{

  struct Config
  {
    constexpr const static task2::E_verbosityLevel default_verbosity_level
        = task2::E_verbosityLevel::INF;
    constexpr const static std::array default_response_file = std::to_array("response.txt");
    constexpr const static uint16_t default_port = 8001;
    constexpr const static uint64_t default_amount_coro_workers = 10;
    constexpr const static uint64_t default_threads = 1;

    std::string response_file = std::string(default_response_file.data());
    uint64_t amount_coro_workers = default_amount_coro_workers;
    std::size_t threads = default_threads;
    uint16_t port = default_port;
    task2::E_verbosityLevel verbosity_level = default_verbosity_level;

  } __attribute__((aligned(64)));  // clangd's proposal

}  // namespace task2server

#endif /* TASK1_CONFIG_HPP */
