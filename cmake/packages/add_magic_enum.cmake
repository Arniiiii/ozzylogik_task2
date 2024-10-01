include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(magic_enum_VERSION 0.9.6)

CPMAddPackage(
  NAME magic_enum
  VERSION ${magic_enum_VERSION}
  # GITHUB_REPOSITORY Neargye/magic_enum GIT_TAG x.y.z # Where `x.y.z` is the release version you
  # want to use.
  URL "https://github.com/Neargye/magic_enum/archive/refs/tags/v${magic_enum_VERSION}.tar.gz"
)
