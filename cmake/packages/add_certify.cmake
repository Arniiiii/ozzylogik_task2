include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(certify_VERSION 0.2)
CPMAddPackage(
  NAME certify
  VERSION ${certify_VERSION}
  GIT_REPOSITORY https://github.com/Arniiiii/certify_cmake
  GIT_TAG testing
  OPTIONS "certify_BUILD_TESTS OFF;certify_BUILD_EXAMPLES OFF"
)
