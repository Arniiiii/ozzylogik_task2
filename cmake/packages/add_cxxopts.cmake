include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(cxxopts_VERSION 3.2.0)
CPMAddPackage(
  NAME cxxopts
  # GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION ${cxxopts_VERSION}
  URL "https://github.com/jarro2783/cxxopts/archive/refs/tags/v${cxxopts_VERSION}.tar.gz"
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)
