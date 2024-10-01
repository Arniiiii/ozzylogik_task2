include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(fmtlog_VERSION 2.3.0)
CPMAddPackage(
  NAME fmtlog
  VERSION ${fmtlog_VERSION}
  GIT_REPOSITORY https://github.com/Arniiiii/fmtlog_cmake_fix.git
  GIT_TAG master
  OPTIONS "fmtlog_ENABLE_CPM ON" "fmtlog_CLANG_FORMAT_BINARY OFF" "fmtlog_WARNINGS_AS_ERRORS OFF"
          "fmtlog_USE_HEADER_ONLY_FMTLIB OFF"
)
