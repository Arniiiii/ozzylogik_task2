cmake_minimum_required(VERSION 3.14...3.22)

project(
  Task2Server
  VERSION 1.0
  LANGUAGES CXX
)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/getCPM.cmake)

# ---- Create standalone executable ----

file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp)
file(GLOB headers CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_executable(${PROJECT_NAME} ${sources} ${headers})
target_include_directories(
  ${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  # $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 26 OUTPUT_NAME "${PROJECT_NAME}")

CPMAddPackage(NAME Task2 SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
target_link_libraries(${PROJECT_NAME} PRIVATE Task2::Task2)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages/add_cxxopts.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE cxxopts::cxxopts)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages/add_fmt.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt-header-only)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages/add_constexpr-to-string.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE constexpr-to-string)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages/add_magic_enum.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE magic_enum::magic_enum)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages/add_fmtlog.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE fmtlog::fmtlog)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages/add_boost.cmake)

set(TRY_BOOST_VERSION "1.86.0")
set(BOOST_NOT_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED "thread;")
set(BOOST_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED "asio;beast;cobalt")
set(BOOST_MY_OPTIONS "BOOST_ENABLE_PYTHON ON;")

add_boost(TRY_BOOST_VERSION BOOST_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED
          BOOST_NOT_HEADER_ONLY_COMPONENTS_THAT_YOU_NEED ${PROJECT_NAME}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_ASIO_HAS_FILE=1)
if(LINUX)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(liburing liburing)

  message(DEBUG "liburing_FOUND : ${liburing_FOUND}")
  message(DEBUG "liburing_LIBRARIES: ${liburing_LIBRARIES}")
  message(DEBUG "liburing_LINK_LIBRARIES: ${liburing_LINK_LIBRARIES}")
  message(DEBUG "liburing_LIBRARY_DIRS: ${liburing_LIBRARY_DIRS}")
  message(DEBUG "liburing_INCLUDE_DIRS: ${liburing_INCLUDE_DIRS}")
  message(DEBUG "liburing_INCLUDE_DIR: ${liburing_INCLUDE_DIR}")
  message(DEBUG "liburing_LDFLAGS: ${liburing_LDFLAGS}")
  message(DEBUG "liburing_LDFLAGS_OTHER: ${liburing_LDFLAGS_OTHER}")
  message(DEBUG "liburing_CFLAGS: ${liburing_CFLAGS}")
  message(DEBUG "liburing_CFLAGS_OTHER: ${liburing_CFLAGS_OTHER}")
  message(DEBUG "liburing_INCLUDEDIR: ${liburing_INCLUDEDIR}")
  message(DEBUG "liburing_LIBDIR: ${liburing_LIBDIR}")
  message(DEBUG "liburing_PREFIX: ${liburing_PREFIX}")

  if(${liburing_FOUND})
    target_include_directories(${PROJECT_NAME} PRIVATE ${liburing_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${liburing_LIBRARIES})
    target_compile_definitions(
      ${PROJECT_NAME} PRIVATE BOOST_ASIO_HAS_IO_URING_AS_DEFAULT=1 BOOST_ASIO_HAS_IO_URING=1
    )
    set(liburing_PACKAGEPROJECT "liburing")
  endif()
endif()

# ---- Create an installable target ----
# this allows users to install and find the library via `find_project()`.

include(../cmake/packages/add_packageproject.cmake)

string(TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)
string(TOLOWER ${PROJECT_NAME}/export.h EXPORT_HEADER_LOCATION)

set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})

packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}
  VERSION_HEADER "${VERSION_HEADER_LOCATION}" EXPORT_HEADER "${EXPORT_HEADER_LOCATION}"
  BINARY_DIR ${PROJECT_BINARY_DIR}
  COMPATIBILITY "AnyNewerVersion" DISABLE_VERSION_SUFFIX ON
  DEPENDENCIES
    "Task2 ${Task2_VERSION};cxxopts ${cxxopts_VERSION};fmt ${fmt_VERSION};constexpr-to-string ${constexpr-to-string_VERSION};magic_enum ${magic_enum_VERSION};fmtlog ${fmtlog_VERSION};${liburing_PACKAGEPROJECT};"
)
