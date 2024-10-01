include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(constexpr-to-string_VERSION 1.0)
CPMAddPackage(
  NAME constexpr-to-string
  VERSION ${constexpr-to-string_VERSION}
  GIT_REPOSITORY https://github.com/Arniiiii/constexpr-to-string-cmake
  GIT_TAG master
)
