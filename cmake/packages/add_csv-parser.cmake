include(${CMAKE_CURRENT_LIST_DIR}/../getCPM.cmake)

set(csv-parser_VERSION 2.3.0)
CPMAddPackage(
  NAME csv
  VERSION ${csv-parser_VERSION}
  # URL "https://github.com/vincentlaucsb/csv-parser/archive/refs/tags/${csv-parser_VERSION}.tar.gz"
  GIT_REPOSITORY "https://github.com/Arniiiii/csv-parser.git"
  GIT_TAG "master"
)
