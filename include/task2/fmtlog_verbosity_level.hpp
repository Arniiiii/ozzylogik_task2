#ifndef TASK1_TASK1_FMTLOG_RELATED_LOGLEVEL_MAP_HPP
#define TASK1_TASK1_FMTLOG_RELATED_LOGLEVEL_MAP_HPP

#include <cstdint>

#include "fmtlog/fmtlog.h"

namespace task2
{

  enum E_verbosityLevel : std::uint8_t
  {
    OFF = 0,
    ERR = 1,
    WRN = 2,
    INF = 3,
    DBG = 4,
  };

  constexpr inline fmtlog::LogLevel verbosityLevelMap(task2::E_verbosityLevel level)
  {
    switch (level)
      {
        case task2::E_verbosityLevel::OFF:
          return fmtlog::LogLevel::OFF;
        case task2::E_verbosityLevel::ERR:
          return fmtlog::LogLevel::ERR;
        case task2::E_verbosityLevel::WRN:
          return fmtlog::LogLevel::WRN;
        case task2::E_verbosityLevel::INF:
          return fmtlog::LogLevel::INF;
        case task2::E_verbosityLevel::DBG:
          return fmtlog::LogLevel::DBG;
      }
  }

}  // namespace task2
#endif /* TASK1_TASK1_FMTLOG_RELATED_LOGLEVEL_MAP_HPP */
