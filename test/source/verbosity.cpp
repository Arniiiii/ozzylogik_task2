#include "gtest/gtest.h"

#include "task2/fmtlog_verbosity_level.hpp"

TEST(FmtlogRelatedLoglevelMap, int_to_enum)
{
  EXPECT_EQ(task2::E_verbosityLevel::OFF, 0);
  EXPECT_EQ(task2::E_verbosityLevel::ERR, 1);
  EXPECT_EQ(task2::E_verbosityLevel::WRN, 2);
  EXPECT_EQ(task2::E_verbosityLevel::INF, 3);
  EXPECT_EQ(task2::E_verbosityLevel::DBG, 4);
}

TEST(FmtlogRelatedLoglevelMap, int_to_fmtlog_enum)
{
  EXPECT_EQ(fmtlog::LogLevel::OFF, task2::verbosityLevelMap(task2::E_verbosityLevel(0)));
  EXPECT_EQ(fmtlog::LogLevel::ERR, task2::verbosityLevelMap(task2::E_verbosityLevel(1)));
  EXPECT_EQ(fmtlog::LogLevel::WRN, task2::verbosityLevelMap(task2::E_verbosityLevel(2)));
  EXPECT_EQ(fmtlog::LogLevel::INF, task2::verbosityLevelMap(task2::E_verbosityLevel(3)));
  EXPECT_EQ(fmtlog::LogLevel::DBG, task2::verbosityLevelMap(task2::E_verbosityLevel(4)));
}

TEST(FmtlogRelatedLoglevelMap, enum_to_fmtlog_enum)
{
  EXPECT_EQ(fmtlog::LogLevel::OFF, task2::verbosityLevelMap(task2::E_verbosityLevel::OFF));
  EXPECT_EQ(fmtlog::LogLevel::ERR, task2::verbosityLevelMap(task2::E_verbosityLevel::ERR));
  EXPECT_EQ(fmtlog::LogLevel::WRN, task2::verbosityLevelMap(task2::E_verbosityLevel::WRN));
  EXPECT_EQ(fmtlog::LogLevel::INF, task2::verbosityLevelMap(task2::E_verbosityLevel::INF));
  EXPECT_EQ(fmtlog::LogLevel::DBG, task2::verbosityLevelMap(task2::E_verbosityLevel::DBG));
}
