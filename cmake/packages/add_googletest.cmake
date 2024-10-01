include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(googletest_VERSION 1.15.2)
CPMAddPackage(
  NAME googletest
  # GITHUB_REPOSITORY google/googletest GIT_TAG release-${googletest_VERSION}
  VERSION ${googletest_VERSION}
  URL "https://github.com/google/googletest/archive/refs/tags/v${googletest_VERSION}.tar.gz"
  OPTIONS "INSTALL_GTEST ON" "gtest_force_shared_crt ON"
)
