#ifndef TASK1_CONFIG_CMD_HPP
#define TASK1_CONFIG_CMD_HPP

#include <type_traits>

#include "fmt/ranges.h"
#include "fmtlog/fmtlog.h"

#include "constexpr-to-string/to_string.hpp"
#include "cxxopts.hpp"
#include "magic_enum.hpp"

#include "task2/fmtlog_verbosity_level.hpp"
#include "task2client/config.hpp"
#include "task2client/version.h"

inline int setConfigViaCmd(task2client::Config &config, int argc, char **argv, char ** /*envp*/)
{
  cxxopts::Options options(
      "Task2Client",
      R"(Task2Client program is expected to do some stuff that Ozzylogik company asked me to do as a test of my knowledge.
To make a client program that connects to the TCP server (like in Task2Server), receives a file, saves it to the hard disk, and exits.
    Translated from Ukrainian via deepl.com
  )");
  options.add_options()("d,destination", "Destination's IP address or domain to connect to",
                        cxxopts::value<std::string>()->default_value(
                            task2client::Config::default_destination.data()))(
      "p,port", "Port",
      cxxopts::value<uint16_t>()->default_value(
          to_string<task2client::Config::default_port>.data()))(
      "r,response-file", "Path to a response file",
      cxxopts::value<std::string>()->default_value(
          task2client::Config::default_response_file.data()))(
      "v,verbosity-level",
      "Verbosity level of logs.\n"
      "  Levels: DBG 4 \n"
      "          INF 3 \n"
      "          WRN 2 \n"
      "          ERR 1 \n"
      "          OFF 0 \n",
      cxxopts::value<std::underlying_type_t<task2::E_verbosityLevel>>()->default_value(
          std::to_string(magic_enum::enum_integer(task2client::Config::default_verbosity_level))))(
      "h,help", "Get some help")(
      "V,version", fmt::format("Get version. Current version: {}", TASK2CLIENT_VERSION));

  auto result = options.parse(argc, argv);

  config.verbosity_level = static_cast<task2::E_verbosityLevel>(
      result["verbosity-level"].as<std::underlying_type_t<task2::E_verbosityLevel>>());
  fmtlog::setLogLevel(verbosityLevelMap(task2::E_verbosityLevel(config.verbosity_level)));

  if ((result.arguments().empty() && result.unmatched().empty()))
    {
      logd("No arguments changed. OK, we'll live with defaults...");
    }

  if (result.count("help") != 0U)
    {
      fmt::println("{}", options.help());
      return 1;
    }

  if (result.count("version") != 0U)
    {
      fmt::println("Task2Client {}", TASK2CLIENT_VERSION);
      return 1;
    }

  if (!result.unmatched().empty())
    {
      loge("There's unmatched arguments : {}\n", result.unmatched());
      fmt::println("{}", options.help());
      return std::make_error_code(std::errc::argument_out_of_domain).value();
    };

  config.port = result["port"].as<std::uint16_t>();
  config.response_file = result["response-file"].as<std::string>();

  return 0;
}

#endif /* TASK1_CONFIG_CMD_HPP */
